#ifndef HEAP_H
#define HEAP_H

# ifndef HEAP_DATA_TYPE
# define HEAP_DATA_TYPE int
# endif

#ifndef HEAP_LESS_THAN
#define HEAP_LESS_THAN(a, b) ((a) < (b))
#endif

#include <stdlib.h>

typedef struct Heap_ Heap, *HeapPtr;

struct Heap_ {
    HEAP_DATA_TYPE *prev; // 指向堆顶的前一个
    HEAP_DATA_TYPE *data;
    int size;
    int capacity;
};

static void heapPercolateDown(const HeapPtr heap, const int top, const int last) {
    HEAP_DATA_TYPE *const prev = heap->prev;
    HEAP_DATA_TYPE const topValue = prev[top];

    int father = top;
    for (int child; (child = father << 1) <= last; father = child) {
        if (child != last && HEAP_LESS_THAN(prev[child + 1], prev[child]))
            child++;
        if (HEAP_LESS_THAN(prev[child], topValue))
            prev[father] = prev[child];
        else
            break;
    }
    prev[father] = topValue;
}

static inline void heapInit(const HeapPtr heap, const int capacity) {
    heap->data = malloc(capacity * sizeof(HEAP_DATA_TYPE));
    heap->prev = heap->data - 1;
    heap->size = 0;
    heap->capacity = capacity;
}

static void heapPush(const HeapPtr heap, HEAP_DATA_TYPE const data) {
    int i, child;
    for (i = ++heap->size; i && HEAP_LESS_THAN(data, heap->prev[child = i >> 1]); i = child)
        heap->prev[i] = heap->prev[child];

    heap->prev[i] = data;
}

static inline void heapPop(const HeapPtr heap, HEAP_DATA_TYPE *const ptr) {
    if(ptr)
        *ptr = *heap->data;

    *heap->data = heap->prev[heap->size];
    heapPercolateDown(heap, 1, --heap->size);
}

static inline HEAP_DATA_TYPE *heapTop(const Heap *const heap) {
    return heap->data;
}

static inline int heapEmpty(const Heap *const heap) {
    return heap->size == 0;
}

static inline void heapFreeData(const HeapPtr heap) {
    free(heap->data);
}

static void buildHeap(const HeapPtr heap, HEAP_DATA_TYPE *const data, const int size) {
    heap->data = data;
    heap->prev = data - 1;
    heap->capacity = heap->size = size;
    for (int i = size >> 1; i; i--)
        heapPercolateDown(heap, i, size);
}

#endif //HEAP_H
