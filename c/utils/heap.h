#ifndef HEAP_H
#define HEAP_H

# ifndef HEAP_ELEMENT_TYPE
# define HEAP_ELEMENT_TYPE int
# endif

#ifndef HEAP_LESS_THAN
#define HEAP_LESS_THAN(a, b) ((a) < (b))
#endif

#include <stdlib.h>

typedef struct Heap_ Heap, *HeapPtr;

struct Heap_ {
    int capacity;
    int size;
    HEAP_ELEMENT_TYPE *prev; // 指向堆顶的前一个
    HEAP_ELEMENT_TYPE *data;
};

static void heapPercolateDown(HEAP_ELEMENT_TYPE *prev, int father, int end) {
    HEAP_ELEMENT_TYPE const theTop = prev[father];

    for (int child; (child = father << 1) <= end; father = child) {
        if (child != end && HEAP_LESS_THAN(prev[child + 1], prev[child]))
            child++;
        if (HEAP_LESS_THAN(prev[child], theTop))
            prev[father] = prev[child];
        else
            break;
    }
    prev[father] = theTop;
}

static HeapPtr newHeap(const int capacity) {
    const HeapPtr heap = malloc(sizeof(Heap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->data = malloc(capacity * sizeof(HEAP_ELEMENT_TYPE));
    heap->prev = heap->data - 1;

    return heap;
}

static void heapInsert(const HeapPtr heap, HEAP_ELEMENT_TYPE const element) {
    int i, child;
    for (i = ++heap->size; i && HEAP_LESS_THAN(element, heap->prev[child = i >> 1]); i = child)
        heap->prev[i] = heap->prev[child];

    heap->prev[i] = element;
}

static HEAP_ELEMENT_TYPE heapDeleteMin(const HeapPtr heap) {
    HEAP_ELEMENT_TYPE const theMin = heap->prev[1];
    heap->prev[1] = heap->prev[heap->size];
    heapPercolateDown(heap->prev, 1, --heap->size);

    return theMin;
}

static void heapDestroy(const HeapPtr heap) {
    free(heap->data);
    free(heap);
}

static void buildHeap(const HeapPtr heap, const int end) {
    for (int i = end >> 1; i; i--)
        heapPercolateDown(heap->prev, i, end);
    heap->size = end;
}

#endif //HEAP_H
