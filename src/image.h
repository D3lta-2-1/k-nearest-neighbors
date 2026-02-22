#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

#define COTE_IMAGE 28

typedef struct Image_t {
	uint8_t pixels [COTE_IMAGE][COTE_IMAGE];
	int label;
} Image;

typedef struct PixelCoordinate_t {
	uint32_t x;
	uint32_t y;
} PixelCoordinate;

Image* read_images(const char* image_file_name, const char* label_file_name, int* nb_images);
void print_image_number(Image* img);
void print_image_threshold(Image* img, uint8_t threshold);
void print_image_degrade(Image* img);
uint8_t get_pixel(Image* img, PixelCoordinate c);
int image_euclid_squared(Image* im1, Image* im2);

#endif IMAGE_H