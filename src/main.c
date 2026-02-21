
#include "image.h"
#include "heap.h"
#include <stdio.h>

int delta(uint8_t* im1, uint8_t* im2) {
    int acc = 0;
    for (int i = 0; i < COTE_IMAGE * COTE_IMAGE; i++) {
        int diff = (int)im1[i] - (int)im2[i];
        acc += diff * diff;
    }
    return acc;
}

int main() {
    int nb_entr;

    Image* images_entr = read_images("images_entr", "etiquettes_entr", &nb_entr);
    if (!images_entr) {
        printf("Erreur de lecture MNIST.\n");
        return 1;
    }

    int nb_test;

    Image* images_test = read_images("images_test", "etiquettes_test", & nb_test);
    if (!images_test) {
        printf("Erreur de lecture MNIST.\n");
        return 1;
    }

    printf("%i images tests trouvees\n", nb_test);
    printf("la premiere entree est: %i\n", images_entr[0].label);
    print_image_threshold(&images_entr[0], 127);

    return 0;
}
