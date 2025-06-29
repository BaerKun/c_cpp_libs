#include "graph/adj_list/min_spanning_tree.h"
#include <stdlib.h>
#include <string.h>

#define HEAP_ELEM_TYPE WeightType *
#define HEAP_LESS_THAN(a, b) (*a < *b)
#include "heap.h"

void PrimMinSpanningTree(const Graph *graph, const WeightType weight[],
                         GraphId predecessor[], const GraphId root) {
  Heap heap;
  heapInit(&heap, graph->vertNum);
  GraphBool *visited = calloc(graph->vertCap, sizeof(GraphBool));
  WeightType *minWeight = malloc(graph->vertCap * sizeof(WeightType));
  memset(minWeight, 0x7f, graph->vertCap * sizeof(WeightType));

  predecessor[root] = -1;
  heapPush(&heap, minWeight + root);
  while (heap.size) {
    const GraphId from = (GraphId)(*heapTop(&heap) - minWeight);
    heapPop(&heap);
    visited[from] = 1;

    for (GraphEdgePtr edge = graph->adjList[from]; edge; edge = edge->next) {
      const GraphId to = edge->to;

      if (!visited[to] && minWeight[to] > weight[edge->id]) {
        minWeight[to] = weight[edge->id];
        predecessor[to] = from;
        heapPush(&heap, minWeight + to);
      }
    }
  }

  free(visited);
  free(minWeight);
  heapFreeData(&heap);
}
