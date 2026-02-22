#include "heap.h"
#include "utilities.h"
#include <stdlib.h>
#include <assert.h>

int last_element(BinaryHeap* heap) {
	return heap->size - 1;
}

int priority(BinaryHeap* heap, int i) {
	return heap->elements[i].priority;
}

void heap_swap(BinaryHeap* heap, int i, int j) {
	assert(i < heap->size && "out of bounds");
	assert(j < heap->size && "out of bounds");
	swap(heap->elements[i], heap->elements[j], HeapElement);
}

void shift_up(BinaryHeap* heap, int i) {
	if (i == 0) return;
	int f = father(i);
	if (priority(heap, f) >= priority(heap, i)) return;
	heap_swap(heap, f, i);
	shift_up(heap, f);
}

void shift_down(BinaryHeap* heap, int i) {
	int left = left_child(i);
	int right = right_child(i);
	int j;
	if (left >= heap->size) return; // no child at all
	if (right >= heap->size) { // there is only a left child, so it is the largest
		j = left;
	}
	else {
		j = priority(heap, left) > priority(heap, right) ? left : right;
	}

	if (priority(heap, j) > priority(heap, i)) {
		heap_swap(heap, left, i);
		shift_down(heap, left);
	}
}

BinaryHeap new_binary_heap(int n) {
	BinaryHeap heap;
	heap.elements = malloc(sizeof(HeapElement) * n);
	heap.size = 0;
	heap.capacity = n;
	return heap;
}

void delete_heap(BinaryHeap* heap) {
	free(heap->elements);
}

bool heap_is_empty(BinaryHeap* heap) {
	return heap->size == 0;
}

bool heap_is_full(BinaryHeap* heap) {
	return heap->capacity == heap->size;
}

void heap_insert(BinaryHeap* heap, int p, Image* v) {
	assert(!heap_is_full(heap) && "heap full");
	HeapElement e = { .priority = p, .value = v };
	heap->elements[heap->size] = e;
	shift_up(heap, heap->size++);
}

HeapElement heap_peek(BinaryHeap* heap) {
	assert(!heap_is_empty(heap) && "heap empty");
	return heap->elements[0];
}

void heap_clear_first(BinaryHeap* heap) {
	if (heap_is_empty(heap)) return;
	heap_swap(heap, 0, last_element(heap));
	heap->size--;
	shift_down(heap, 0);
}

void heap_clear(BinaryHeap* heap) {
	heap->size = 0;
}

HeapElement heap_get(BinaryHeap* heap) {
	HeapElement tmp = heap_peek(heap);
	heap_clear_first(heap);
	return tmp;
}

#ifdef TESTS
#include <utest.h>

UTEST_F_SETUP(BinaryHeap_t) {
	*utest_fixture = new_binary_heap(4);
}

UTEST_F_TEARDOWN(BinaryHeap_t) {
	delete_heap(utest_fixture);
}

UTEST_F(BinaryHeap_t, insertion) {
	heap_insert(utest_fixture, 5, NULL);
	heap_insert(utest_fixture, 4, NULL);
	heap_insert(utest_fixture, 3, NULL);
	heap_insert(utest_fixture, 2, NULL);
	ASSERT_TRUE(heap_is_full(utest_fixture));
}

UTEST_F(BinaryHeap_t, get) {
	heap_insert(utest_fixture, 5, NULL);
	heap_insert(utest_fixture, 3, NULL);
	heap_insert(utest_fixture, 4, NULL);
	heap_insert(utest_fixture, 7, NULL);

	ASSERT_EQ(utest_fixture->size, 4);

	HeapElement first = heap_get(utest_fixture);
	ASSERT_EQ(first.priority, 7);
	ASSERT_EQ(utest_fixture->size, 3);

	HeapElement second = heap_get(utest_fixture);
	ASSERT_EQ(second.priority, 5);
	ASSERT_EQ(utest_fixture->size, 2);

	HeapElement third = heap_get(utest_fixture);
	ASSERT_EQ(third.priority, 4);
	ASSERT_EQ(utest_fixture->size, 1);

	HeapElement last = heap_get(utest_fixture);
	ASSERT_EQ(last.priority, 3);
	ASSERT_TRUE(heap_is_empty(utest_fixture));
}

#endif