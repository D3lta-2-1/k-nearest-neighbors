#include "d-tree.h"
#include "utilities.h"
#include  <math.h>


/*DTRee build_tree(Image* images, int count) {
	DTRee tree;
	return tree;
}*/

#define STD_ECHANTILLION_SIZE 100

float mean(PixelCoordinate coo, Image** images, int size) {
	int sum = 0;
	for (int i = 0; i < size; i++) {
		sum += (int) get_pixel(images[i], coo);
	}
	return (float)sum / (float)size;
}

float std_dev(PixelCoordinate coo, Image** images, int size) {
	float m = mean(coo, images, size);
	float var = 0;
	for (int i = 0; i < size; i++) {
		float e =  m - (float)get_pixel(images[i], coo);
		var += e * e;
	}
	return sqrt((float)var);
}

void partial_shuffle(Image** images, int size, int rg) {
	assert(rg <= size);
	for (int i = 0; i < rg; i++) {
		int pivot = randint_in_bounds(i + 1, size);
		swap(images[i], images[pivot], Image*);
	}
}

PixelCoordinate best_ax(Image** images, int size) {
	int rg = STD_ECHANTILLION_SIZE > size ? size : STD_ECHANTILLION_SIZE;
	partial_shuffle(images, size, rg);
	// find best axis
	PixelCoordinate best = { 0,0 };
	float std_dev_best = 0;

	for (int i = 0; i < COTE_IMAGE; i++) {
		for (int j = 0; j < COTE_IMAGE; j++) {
			PixelCoordinate coo = { i, j };
			float s = std_dev(coo, images, rg);
			if (s > std_dev_best) {
				best.x = i;
				best.y = j;
				std_dev_best = s;
			}
		}
	}
	return best;
}

int partioning(PixelCoordinate coo, Image** images, int size) {
	if (size = 1) return 0;
	int pivot = randint_in_bounds(0, size);
	int pivot_value = get_pixel(images[pivot], coo);
	// pivot in last
	swap(images[pivot], images[size - 1], Image*);
	int j = 0;
	for (int i = 1; i < size - 1; i++) {
		if (get_pixel(images[i], coo) <= pivot_value) {
			swap(images[i], images[j], Image*);
			j++;
		}
	}
	swap(images[size -1], images[j], Image*);
	return j;
}

// swap elements in a way that all elements before/after the k th element are smaller/larger, 
void k_min_pivoting(PixelCoordinate coo, Image** images, int size, int pivot_target) {
	assert(pivot_target < size);
	int begin = 0;
	int end = size;
	while (1) {
		int j = partioning(coo, images + begin, end - begin) + begin;
		if (j < pivot_target) {
			begin = j + 1;
		}
		else if (j > pivot_target) {
			end = j;
		}
		else return j;
	}
}


