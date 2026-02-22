#ifndef D_TREE_H
#define D_TREE_H

#include "image.h"
#include "heap.h"

typedef struct DTRee_t {
	Image** image_tree;
	PixelCoordinate* speration_tree;
	int image_tree_size; // because image_tree is a complete tree N = (2n - 1), speration_tree size is n - 1 = N + 1 / 2
} DTRee;

DTRee new_tree(Image* images, int count);
void delete_tree(DTRee* tree);
void dtree_find_closest(DTRee* tree, BinaryHeap* heap, Image* point);

#endif D_TREE_H

