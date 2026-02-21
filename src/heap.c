#include "heap.h"
#include <stdlib.h>
#include <assert.h>
#include "utilities.h"

int last_element(BinaryHeap* heap) {
	return heap->size - 1;
}

void heap_swap(BinaryHeap* heap, int i, int j) {
	assert(i < heap->size && "out of bounds");
	assert(j < heap->size && "out of bounds");
	swap(heap->elements[i], heap->elements[j], HeapElement);
}

void shift_up(BinaryHeap* heap, int i) {
	if (i == 0) return;
	int f = father(i);
	if (heap->elements[f].priority >= heap->elements[i].priority) return;
	heap_swap(heap, f, i);
	shift_up(heap, father);
}

void shift_down(BinaryHeap* heap, int i) {
	int left = left_child(i);
	if (left >= heap->capacity) return; // no left child
	if (heap->elements[i].priority <= heap->elements[left].priority) {
		heap_swap(heap, left, i);
		shift_down(heap, left);
		return;
	}
	int right = right_child(i);
	if (right >= heap->capacity) return;
	if (heap->elements[i].priority <= heap->elements[right].priority) {
		heap_swap(heap, right, i);
		shift_down(heap, right);
		return;
	}
}

BinaryHeap new_binary_heap(int n) {
	BinaryHeap tas;
	tas.elements = malloc(sizeof(HeapElement) * n);
	tas.size = 0;
	tas.capacity = n;
	return tas;
}

bool is_empty(BinaryHeap* heap) {
	return heap->size == 0;
}

bool is_full(BinaryHeap* heap) {
	return heap->capacity == heap->size;
}

void insert(BinaryHeap* heap, int p, int v) {
	assert(!is_full(heap) && "heap full");
	HeapElement e = { .priority = p, .value = v };
	heap->elements[heap->size] = e;
	shift_up(heap, heap->size++);
}

HeapElement peek(BinaryHeap* heap) {
	assert(!is_empty(heap) && "heap empty");
	return heap->elements[0];
}

void clear_first(BinaryHeap* heap) {
	if (is_empty(heap)) return;
	heap_swap(heap, 0, last_element(heap));
	heap->size--;
	shift_down(heap, 0);
}

HeapElement get(BinaryHeap* heap) {
	HeapElement tmp = peek(heap);
	clear_first(heap);
	return tmp;
}

