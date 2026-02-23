#include "k-mean.h"
#include "utilities.h"
#include <stdlib.h>
#include <stdio.h>

int closest(Image* image, Image* images, int n) {
	int closest = 0;
	int min_distance = INT_MAX;
	for (int i = 0; i < n; i++) {
		int distance = image_euclid_squared(image, &images[i]);
		if (distance < min_distance) {
			closest = i;
			min_distance = distance;
		}
	}
	return closest;
}

void classify(Image* images, int n, Image* barycenters, int k, int* output) {
	for (int i = 0; i < n; i++) {
		output[i] = closest(&images[i], barycenters, k);
	}
}

void compute_berycenter(Image* images, int* classes, int n, int k, Image* barycenters) {
	for (int i = 0; i < k; i++) {
		barycenters[i].label = k; // it's implemented, it's better than leaving it empty...

		// init vector
		int pixels[COTE_IMAGE][COTE_IMAGE];
		for (int l = 0; l < COTE_IMAGE; l++) {
			for (int m = 0; m < COTE_IMAGE; m++) {
				pixels[l][m] = 0;
			}
		}

		//compute the new barycenter
		int count = 0;
		for (int j = 0; j < n; j++) {
			if (classes[j] != i) continue;
			count++;
			for (int l = 0; l < COTE_IMAGE; l++) {
				for (int m = 0; m < COTE_IMAGE; m++) {
					pixels[l][m] += images->pixels[l][m];
				}
			}
		}

		for (int l = 0; l < COTE_IMAGE; l++) {
			for (int m = 0; m < COTE_IMAGE; m++) {
				if (count != 0) {
					barycenters[i].pixels[l][m] = pixels[l][m] / count;
				}
				else {
					barycenters[i].pixels[l][m] = 0;
				}
				
			}
		}
	}
}

int* k_mean(int k, Image* images, int n, int nb_iter) {
	Image* barycenters = (Image*)malloc(sizeof(Image) * k);
	int* classes = (int*)malloc(sizeof(int) * n);

	int* starting = n_choose_k(k, n);
	for (int i = 0; i < k; i++) {
		printf("%i\n", starting[i]);
		barycenters[i] = images[starting[i]];
	}
	free(starting);
	classify(images, n, barycenters, k, classes);

	for (int iter = 0; iter < nb_iter; iter++) {
		compute_berycenter(images, classes, n, k, barycenters);
		classify(images, n, barycenters, k, classes);
	}
	free(barycenters);
	return classes;
}

int** count_occ(Image* images, int* classes, int n, int k) {
	int** output = malloc(sizeof(int*) * k);
	for (int i = 0; i < k; i++) {
		output[i] = malloc(sizeof(int) * 10);
	}

	for (int i = 0; i < k; i++) {
		for (int j = 0; j < 10; j++) {
			output[i][j] = 0;
		}
	}

	for (int j = 0; j < n; j++) {
		output[classes[n]][images[n].label]++;
	}

	return output;
}