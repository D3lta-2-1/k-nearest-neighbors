#include "d-tree.h"
#include "utilities.h"
#include  <math.h>

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
	int end = (rg == size) ? size - 1 : rg;
	for (int i = 0; i < end; i++) {
		assert(i + 1 < size);
		int pivot = randint_in_bounds(i + 1, size);
		swap(images[i], images[pivot], Image*);
	}
}

PixelCoordinate find_best_axe(Image** images, int size) {
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
	if (size == 1) return 0;
	int pivot = randint_in_bounds(0, size);
	int pivot_value = get_pixel(images[pivot], coo);
	// pivot in last
	swap(images[pivot], images[size - 1], Image*);
	int j = 0;
	for (int i = 0; i < size - 1; i++) {
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
		else return;
	}
}

int hyperfloor2(int n) {
	assert(n > 0);
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;

	return n - (n >> 1);
}

unsigned int left_subtree_size(unsigned int n)
{
	if (n <= 1)
		return 0;

	unsigned int p = hyperfloor2(n);

	// node before last layer
	unsigned int full = p - 1;

	// node in the last layer
	unsigned int last = n - full;

	// max capacity of the last layer on the left
	unsigned int max_left_last = p >> 1;

	// perfect left sub tree 
	unsigned int left = max_left_last - 1;

	// add position of the last layer
	if (last < max_left_last)
		left += last;
	else
		left += max_left_last;

	return left;
}

void build_tree_rec(Image** images_ref,
	int count,
	Image** image_tree,
	PixelCoordinate* separation_tree,
	int pos)
{
	if (count == 0) return;
	if (count == 1) {
		image_tree[pos] = images_ref[0];
		return;
	}

	PixelCoordinate axe = find_best_axe(images_ref, count);
	separation_tree[pos] = axe;

	int left_size = left_subtree_size(count);

	k_min_pivoting(axe, images_ref, count, left_size);

	image_tree[pos] = images_ref[left_size];

	build_tree_rec(images_ref,
		left_size,
		image_tree,
		separation_tree,
		2 * pos + 1);

	build_tree_rec(images_ref + left_size + 1,
		count - left_size - 1,
		image_tree,
		separation_tree,
		2 * pos + 2);
}

DTRee new_tree(Image* images, int count) {
	int image_tree_size = count;
	int speration_tree_size = count / 2;

	Image** images_ref = malloc(image_tree_size * sizeof(Image*));
	assert(images_ref != NULL);
	for (int i = 0; i < image_tree_size; i++) {
		images_ref[i] = &images[i];
	}

	Image** image_tree = malloc(image_tree_size * sizeof(Image*));
	assert(image_tree != NULL);
	for (int i = 0; i < image_tree_size; i++) {
		image_tree[i] = NULL;
	}

	PixelCoordinate* seperation_tree = malloc(speration_tree_size * sizeof(PixelCoordinate));
	assert(seperation_tree != NULL);
	for (int i = 0; i < speration_tree_size; i++) {
		seperation_tree[i].x = -1;
		seperation_tree[i].y = -1;
	}

	build_tree_rec(images_ref, image_tree_size, image_tree, seperation_tree, 0);
	DTRee tree = { .image_tree = image_tree, .speration_tree = seperation_tree, .image_tree_size = image_tree_size };
	free(images_ref);
	return tree;
}

void delete_tree(DTRee* tree) {
	free(tree->image_tree);
	free(tree->speration_tree);
}

#ifdef TESTS
#include <utest.h>
#include <stdbool.h>

UTEST(pivoting, k_min) {
	const int count = 10;
	const int pivot_target = 5;
	PixelCoordinate p = { .x = 0, .y = 0 };
	Image* images_buffer = calloc(count, sizeof(Image));
	assert(images_buffer != NULL);
	for (int i = 0; i < count; i++) {
		images_buffer[i].pixels[p.x][p.y] = count - 1 - i;
	}

	Image** images_ref = malloc(count * sizeof(Image*));
	assert(images_ref != NULL);
	for (int i = 0; i < count; i++) {
		images_ref[i] = &images_buffer[i];
	}

	k_min_pivoting(p, images_ref, count, pivot_target);

	for (int i = 0; i < pivot_target; i++) {
		ASSERT_LE(get_pixel(images_ref[i], p), get_pixel(images_ref[pivot_target], p));
	}

	for (int i = pivot_target + 1; i < count; i++) {
		ASSERT_GE(get_pixel(images_ref[i], p), get_pixel(images_ref[pivot_target], p));
	}
}

UTEST_F_SETUP(DTRee_t) {
	int count;
	Image* images_entr = read_images("images_entr", "etiquettes_entr", &count);
	assert(count > 0 && "failed to load MNIST");
	*utest_fixture = new_tree(images_entr, count);
}

UTEST_F(DTRee_t, valid_memory_layout) {
	int seperation_tree_size = utest_fixture->image_tree_size / 2;

	for (int i = 0; i < seperation_tree_size; i++) {
		PixelCoordinate* p = &utest_fixture->speration_tree[i];
		ASSERT_TRUE(p->x != -1 && p->y != -1);
	}

	for (int i = 0; i < utest_fixture->image_tree_size; i++) {
		ASSERT_NE(utest_fixture->image_tree[i], NULL);
	}
}

bool is_well_build(DTRee* tree, int node) {
	if (node < tree->image_tree_size / 2) {
		PixelCoordinate axe = tree->speration_tree[node];
		int left = left_child(node);
		int right = right_child(node);
		if (left < tree->image_tree_size) {
			if (get_pixel(tree->image_tree[left], axe) > get_pixel(tree->image_tree[node], axe)) return false;
		}
		if (right < tree->image_tree_size) {
			if (get_pixel(tree->image_tree[right], axe) < get_pixel(tree->image_tree[node], axe)) return false;
		}
	}
	return true;
}


UTEST_F(DTRee_t, valid_tree_properties) {
	int seperation_tree_size = utest_fixture->image_tree_size / 2;
	for (int i = 0; i < seperation_tree_size; i++) {
		ASSERT_TRUE(is_well_build(utest_fixture, i));
	}
}


UTEST_F_TEARDOWN(DTRee_t) {
	delete_tree(utest_fixture);
}

#endif

