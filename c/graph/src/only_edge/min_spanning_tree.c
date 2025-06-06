#include "only_edge/min_spanning_tree.h"
#include "disjoint_set.h"

#define HEAP_DATA_TYPE OnlyEdgePtr
#define HEAP_LESS_THAN(a, b) ((a)->weight < (b)->weight)
#include "heap.h"

#include <stdio.h>

void KruskalMinSpanningTree(const OnlyEdgeGraphPtr graph, EdgeId outputArray[]) {
  Heap heap;
  OnlyEdgePtr *heapBuff = malloc(graph->edgeNum * sizeof(OnlyEdgePtr));
  for (int i = 0; i < graph->edgeNum; ++i) heapBuff[i] = graph->edges + i;
  buildHeap(&heap, heapBuff, graph->edgeNum);

  VertexId *disjSet = malloc(graph->vertexNum * sizeof(VertexId));
  disjointSetInit(disjSet, graph->vertexNum);

  int counter = 0;
  while (heap.size != 0) {
    const OnlyEdgePtr edge = *heapTop(&heap);
    heapPop(&heap);

    const int root1 = disjointSetFind(disjSet, edge->vertex1);
    const int root2 = disjointSetFind(disjSet, edge->vertex2);

    if (root1 != root2) {
      outputArray[counter++] = (EdgeId)(edge - graph->edges);
      disjointSetUnion(disjSet, root1, root2);
    }
  }
  if (counter + 1 != graph->vertexNum) printf("No spanning tree!\n");

  outputArray[counter] = -1;
  free(heapBuff);
  free(disjSet);
}
