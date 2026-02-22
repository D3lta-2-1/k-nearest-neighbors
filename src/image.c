#include "image.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

uint32_t bitswap(uint32_t num) {

    // Unsigned 32-bit conversion:
    uint32_t swapped = ((num >> 24) & 0xff) | // move byte 3 to byte 0
        ((num << 8) & 0xff0000) | // move byte 1 to byte 2
        ((num >> 8) & 0xff00) | // move byte 2 to byte 1
        ((num << 24) & 0xff000000); // byte 0 to byte 3
    return swapped;
}

Image* read_images(const char* image_file_name, const char* label_file_name, int* nb_images) {
    FILE* image_file = fopen(image_file_name, "rb");
    if (!image_file) {
        perror("error during file loading");
        return NULL;
    }

    FILE* label_file = fopen(label_file_name, "rb");
    if (!label_file) {
        perror("error during file loading");
        return NULL;
    }

    // big-endian Int reading
    uint32_t image_magic, image_count, rows, cols;

    fread(&image_magic, 4, 1, image_file);
    fread(&image_count, 4, 1, image_file);
    fread(&rows, 4, 1, image_file);
    fread(&cols, 4, 1, image_file);

    // Conversion big-endian -> little-endian if required
    image_magic = bitswap(image_magic);
    image_count = bitswap(image_count);
    rows = bitswap(rows);
    cols = bitswap(cols);

    if (image_magic != 2051) {
        fprintf(stderr, "Error : wrong magic number (%u, 2051 expected)\n", image_magic);
        fclose(image_file);
        fclose(label_file);
        return NULL;
    }

    uint32_t label_magic, label_count;

    fread(&label_magic, 4, 1, label_file);
    fread(&label_count, 4, 1, label_file);

    // Conversion big-endian -> little-endian
    label_magic = bitswap(label_magic);
    label_count = bitswap(label_count);

    if (label_magic != 2049) {
        fprintf(stderr, "Error : mauvais magic number (%u, 2049 expected)\n", label_magic);
        fclose(image_file);
        fclose(label_file);
        return NULL;
    }

    if (label_count != image_count) {
        fprintf(stderr, "Error : mauvais magic number (%u, 2049 expected)\n", label_magic);
        fclose(image_file);
        fclose(label_file);
        return NULL;
    }

    if (rows != COTE_IMAGE || cols != COTE_IMAGE) {
        perror("wrong image size");
        fclose(image_file);
        fclose(label_file);
        return NULL;
    }

    // Buffer temporaire pour lecture brute
    uint8_t* buffer = (uint8_t*)malloc(label_count);
    if (!buffer) {
        perror("Allocation buffer échouée");
        fclose(image_file);
        fclose(label_file);
        return NULL;
    }

    fread(buffer, 1, label_count, label_file);

    Image* images = (Image*)malloc(image_count * sizeof(Image));
    if (!images) {
        perror("Allocation failed");
        fclose(image_file);
        return NULL;
    }

    size_t img_size = COTE_IMAGE * COTE_IMAGE;
    for (uint32_t i = 0; i < image_count; i++) {
        fread(&images[i].pixels, 1, img_size, image_file);
        images[i].label = (int)buffer[i];
    }

    free(buffer);
    fclose(image_file);
    fclose(label_file);

    // data forwarding
    if (nb_images) *nb_images = image_count;

    return images;
}

void print_image_number(Image* img) {
    for (int i = 0; i < COTE_IMAGE; i++) {
        for (int j = 0; j < COTE_IMAGE; j++) {
            int v = img->pixels[i][j];
            if (v < 100) printf("0");
            if (v < 10)  printf("0");
            printf("%d ", v);
        }
        printf("\n");
    }
}

void print_image_threshold(Image* img, uint8_t threshold) {
    for (int i = 0; i < COTE_IMAGE; i++) {
        for (int j = 0; j < COTE_IMAGE; j++) {
            int v = img->pixels[i][j];
            printf("%c ", v > threshold ? 'O' : '.');
        }
        printf("\n");
    }
}

void print_image_degrade(Image* img) {
    for (int i = 0; i < COTE_IMAGE; i++) {
        for (int j = 0; j < COTE_IMAGE; j++) {
            int v = img->pixels[i][j];
            if (v < 52 * 1) printf(" ");
            else if (v < 52 * 2) printf("?");
            else if (v < 52 * 3) printf("?");
            else if (v < 52 * 4) printf("?");
            else               printf("?");
        }
        printf("\n");
    }
}

uint8_t get_pixel(Image* img, PixelCoordinate c) {
    return img->pixels[c.x][c.x];
}

int image_euclid_squared(Image* im1, Image* im2) {
    int acc = 0;
    for (int i = 0; i < COTE_IMAGE; i++) {
        for (int j = 0; j < COTE_IMAGE; j++) {
            int v = (int)im1->pixels[i][j] - (int)im2->pixels[i][j];
            acc += v * v;
        }
    }
    return acc;
}
