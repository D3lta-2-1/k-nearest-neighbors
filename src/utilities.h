#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdlib.h>
#include <assert.h>

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

#endif UTILITIES_H
