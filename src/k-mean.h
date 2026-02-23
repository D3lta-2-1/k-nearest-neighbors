#ifndef K_MEAN_H
#define K_MEAN_H

#include "image.h"
#include <limits.h>

int* k_mean(int k, Image* image, int n, int nb_iter);
int** count_occ(Image* images, int* classes, int n, int k);

#endif K_MEAN_H
