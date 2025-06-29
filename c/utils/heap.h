#ifndef HEAP_H
#define HEAP_H

#ifndef HEAP_ELEM_TYPE
#define HEAP_ELEM_TYPE int
#endif

#ifndef HEAP_LESS_THAN
#define HEAP_LESS_THAN(a, b) ((a) < (b))
#endif

#include <stdlib.h>
#include <stdint.h>

typedef HEAP_ELEM_TYPE HeapDataType_ ;
typedef struct Heap_ Heap, *HeapPtr;

struct Heap_ {
  HeapDataType_ *prev; // 指向堆顶的前一个
  HeapDataType_ *data;
  uint64_t capacity, size;
};

static void heapPercolateDown(const HeapPtr heap, const uint64_t top,
                              const uint64_t last) {
  HeapDataType_ *const prev = heap->prev;
  HeapDataType_ const topValue = prev[top];

  uint64_t father = top;
  for (uint64_t child; (child = father << 1) <= last; father = child) {
    if (child != last && HEAP_LESS_THAN(prev[child + 1], prev[child])) child++;
    if (HEAP_LESS_THAN(prev[child], topValue))
      prev[father] = prev[child];
    else
      break;
  }
  prev[father] = topValue;
}

static inline void heapInit(const HeapPtr heap, const uint64_t capacity) {
  heap->data = (HeapDataType_ *)malloc(capacity * sizeof(HeapDataType_));
  heap->prev = heap->data - 1;
  heap->size = 0;
  heap->capacity = capacity;
}

static void heapPush(const HeapPtr heap, HeapDataType_ const data) {
  int i, child;
  for (i = ++heap->size;
       ((child = i >> 1)) && HEAP_LESS_THAN(data, heap->prev[child]); i = child)
    heap->prev[i] = heap->prev[child];

  heap->prev[i] = data;
}

static inline void heapPop(const HeapPtr heap) {
  *heap->data = heap->prev[heap->size];
  heapPercolateDown(heap, 1, --heap->size);
}

static inline HeapDataType_ *heapTop(const Heap *const heap) {
  return heap->data;
}

static inline int heapEmpty(const Heap *const heap) { return heap->size == 0; }

static inline void heapFreeData(const HeapPtr heap) { free(heap->data); }

static void buildHeap(const HeapPtr heap, HeapDataType_ *const data,
                      const uint64_t size) {
  heap->data = data;
  heap->prev = data - 1;
  heap->capacity = heap->size = size;
  for (uint64_t i = size >> 1; i; --i) heapPercolateDown(heap, i, size);
}

#endif // HEAP_H
