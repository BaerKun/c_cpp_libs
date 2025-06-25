#include "graph/edge_set/min_spanning_tree.h"
#include "disjoint_set.h"
#include <stdio.h>

#define HEAP_DATA_TYPE const WeightType *
#define HEAP_LESS_THAN(a, b) (*a < *b)
#include "heap.h"

void KruskalMinSpanningTree(const GraphEdgeSetElem *set, const WeightType weight[],
                            const GraphSize edgeNum, const GraphSize vertNum,
                            GraphId *tree) {
  Heap heap;
  const WeightType **heapBuff = malloc(edgeNum * sizeof(WeightType *));
  for (int i = 0; i < edgeNum; ++i) {
    heapBuff[i] = weight + i;
  }
  buildHeap(&heap, heapBuff, edgeNum);

  int *disjointSet = malloc(vertNum * sizeof(int));
  disjointSetInit(disjointSet, vertNum);

  int counter = 0;
  while (heap.size != 0) {
    const WeightType *w = *heapTop(&heap);
    heapPop(&heap);

    const GraphId id = w - weight;
    const GraphEdgeSetElem *edge = set + id;
    const int root1 = disjointSetFind(disjointSet, edge->from);
    const int root2 = disjointSetFind(disjointSet, edge->to);

    if (root1 != root2) {
      tree[counter++] = id;
      disjointSetUnion(disjointSet, root1, root2);
    }
  }
  if (counter + 1 != vertNum) printf("No spanning tree!\n");

  free(heapBuff);
  free(disjointSet);
}
