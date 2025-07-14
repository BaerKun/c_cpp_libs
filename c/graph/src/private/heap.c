#include "private/heap.h"
#include <stdlib.h>

void graphHeapPercolateDown(GraphHeap *heap, GraphSize father) {
  const GraphId top = heap->data[father];
  const WeightType topValue = heap->weight[top];

  for (GraphSize child; (child = father << 1) <= heap->size; father = child) {
    if (child != heap->size &&
        heap->weight[heap->data[child + 1]] < heap->weight[heap->data[child]])
      ++child;
    if (heap->weight[heap->data[child]] < topValue)
      heap->data[father] = heap->data[child];
    else
      break;
  }
  heap->data[father] = top;
}

GraphHeap *graphHeapCreate(const GraphSize capacity, const WeightType *weight) {
  GraphHeap *heap = malloc(sizeof(GraphHeap) + capacity * sizeof(GraphId));
  heap->capacity = capacity;
  heap->size = 0;
  heap->weight = weight;
  *heap->data = -1;
  return heap;
}

void graphHeapRelease(GraphHeap *heap) { free(heap); }

void graphHeapPush(GraphHeap *heap, const GraphId id) {
  const WeightType value = heap->weight[id];
  GraphSize child = ++heap->size;
  for (GraphSize father; value < heap->weight[heap->data[father = child << 1]];
       child = father) {
    heap->data[child] = heap->data[father];
  }
  heap->data[child] = id;
}

GraphId graphHeapPop(GraphHeap *heap) {
  GraphId *top = heap->data + 1;
  const GraphId ret = *top;
  *top = heap->data[heap->size--];
  graphHeapPercolateDown(heap, 1);
  return ret;
}

GraphHeap *graphHeapBuild(const GraphSize capacity, const WeightType *weight) {
  GraphHeap *heap = graphHeapCreate(capacity, weight);
  heap->size = capacity;
  for (GraphId i = 0; i != capacity; ++i) heap->data[i + 1] = i;
  for (uint64_t i = capacity >> 1; i; --i) graphHeapPercolateDown(heap, i);
  return heap;
}
