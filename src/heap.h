#ifndef TAS_H
#define TAS_H
#include <stdbool.h>

typedef struct HeapElement_t {
	int priority;
	int value;
} HeapElement;

typedef struct BinaryHeap_t {
	int capacity;
	int size;
	HeapElement* elements;
	
} BinaryHeap;

// create a new binary heap with a fixed capacity, overfilling will trigger an assertion
BinaryHeap new_binary_heap(int n);

// release a heap
void delete_heap(BinaryHeap* heap);

// return weather or not the heap is empty
bool heap_is_empty(BinaryHeap* heap);
bool heap_is_full(BinaryHeap* heap);

// insert an element 
void heap_insert(BinaryHeap* heap, int p, int v);

// get the element with the largest priority without removing it from the heap, assert if empty
HeapElement heap_peek(BinaryHeap* heap);

// remove an element from the heap
void heap_clear_first(BinaryHeap* heap);

// get and remove from the heap an element
HeapElement heap_get(BinaryHeap* heap);

#endif TAS_H

