#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define swap(x, y, T) do { T SWAP = x; x = y; y = SWAP; } while (0)

inline int father(int i) {
	return (i - 1) / 2;
}

inline int left_child(int i) {
	return 2 * i + 1;
}

inline int right_child(int i) {
	return  2 * i + 2;
}

// random number in [a..b[
inline int randint_in_bounds(int min, int max) {
	assert(min < max && "empty range");
	return rand() % (max - min) + min;
}

inline bool contain(int e, int* tab, int size) {
	for (int i = 0; i < size; i++) {
		if (tab[i] == e) return true;
	}
	return false;
}

inline int* n_choose_k(int k, int n) {
	int* array = malloc(sizeof(int) * k);
	for (int i = 0; i < k; i++) {
		int value;
		do {
			value = randint_in_bounds(0, n);
		}
		while (contain(value, array, i));
		array[i] = value;
	}
	return array;
}

#endif UTILITIES_H
