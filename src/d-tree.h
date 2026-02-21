#ifndef D_TREE_H
#define D_TREE_H

#include "image.h"

typedef struct DTreeElemenet_t {
	Image* image;
	int split_x; //axe on which it get splitten
	int split_y;
} DTreeElement;

typedef struct DTRee_t {
	DTreeElement* tree;
	int size;
} DTRee;

//DTRee build_tree(Image* images, int count);

#endif D_TREE_H

