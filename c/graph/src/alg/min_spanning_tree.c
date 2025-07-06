#include "private/graph_detail.h"
#include "graph/iter.h"
#include <stdlib.h>
#include <string.h>

#define HEAP_ELEM_TYPE WeightType *
#define HEAP_LESS_THAN(a, b) (*a < *b)
#include "heap.h"

void PrimMinSpanningTree(const Graph *graph, const WeightType weight[],
                         GraphId predecessor[], const GraphId root) {
  Heap heap;
  heapInit(&heap, graph->vertNum);
  GraphIter *iter = graphGetIter(graph);
  GraphBool *visited = calloc(graph->vertMng.range, sizeof(GraphBool));
  WeightType *minWeight = malloc(graph->vertMng.range * sizeof(WeightType));
  memset(minWeight, UNREACHABLE, graph->vertMng.range * sizeof(WeightType));

  GraphId id, to;
  predecessor[root] = INVALID_ID;
  heapPush(&heap, minWeight + root);
  while (heap.size) {
    const GraphId from = (GraphId)(*heapTop(&heap) - minWeight);
    heapPop(&heap);
    visited[from] = 1;

    while (graphIterNextEdge(iter, from, &id, &to)) {
      if (!visited[to] && minWeight[to] > weight[id]) {
        minWeight[to] = weight[id];
        predecessor[to] = from;
        heapPush(&heap, minWeight + to);
      }
    }
  }

  free(visited);
  free(minWeight);
  graphIterRelease(iter);
  heapFreeData(&heap);
}

#include "disjoint_set.h"

void KruskalMinSpanningTree(const GraphEdgeEndpoint *set,
                            const WeightType weight[], const GraphSize edgeNum,
                            const GraphSize vertNum, GraphId *tree) {
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
    const GraphEdgeEndpoint *edge = set + id;
    const int root1 = disjointSetFind(disjointSet, edge->from);
    const int root2 = disjointSetFind(disjointSet, edge->to);

    if (root1 != root2) {
      tree[counter++] = id;
      disjointSetUnion(disjointSet, root1, root2);
    }
  }
  if (counter + 1 != vertNum) {
    // No spanning tree
  }

  free(heapBuff);
  free(disjointSet);
}
