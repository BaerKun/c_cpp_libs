#include "graph/iter.h"
#include "private/graph_detail.h"
#include "private/heap.h"
#include <stdlib.h>
#include <string.h>

void PrimMinSpanningTree(const Graph *graph, const WeightType weight[],
                         GraphId predecessor[], const GraphId root) {
  const GraphView *view = VIEW(graph);
  GraphIter *iter = graphIterFromView(view);
  GraphBool *visited = calloc(view->vertRange, sizeof(GraphBool));
  WeightType *minWeight = malloc(view->vertRange * sizeof(WeightType));
  GraphHeap *heap = graphHeapCreate(graph->vertNum, minWeight);
  memset(minWeight, UNREACHABLE_BYTE, view->vertRange * sizeof(WeightType));

  GraphId id, to;
  predecessor[root] = INVALID_ID;
  graphHeapPush(heap, root);
  while (!graphHeapEmpty(heap)) {
    const GraphId from = graphHeapPop(heap);
    visited[from] = GRAPH_TRUE;

    while (graphIterNextEdge(iter, from, &id, &to)) {
      if (!visited[to] && minWeight[to] > weight[id]) {
        minWeight[to] = weight[id];
        predecessor[to] = from;
        graphHeapPush(heap, to);
      }
    }
  }

  free(visited);
  free(minWeight);
  graphIterRelease(iter);
  graphHeapRelease(heap);
}

#include "private/disjoint_set.h"

void KruskalMinSpanningTree(const GraphEndpoint *set, const WeightType weight[],
                            const GraphSize edgeNum, const GraphSize vertNum,
                            GraphId *tree) {
  GraphHeap *heap = graphHeapBuild(edgeNum, weight);
  GraphDisjointSet *disjointSet = graphDisjointCreate(vertNum);

  GraphSize counter = 0;
  while (!graphHeapEmpty(heap)) {
    const GraphId id = graphHeapPop(heap);
    const GraphEndpoint *edge = set + id;
    const GraphId cls1 = graphDisjointFind(disjointSet, edge->to);
    const GraphId cls2 = graphDisjointFind(disjointSet, edge->from);

    if (cls1 != cls2) {
      tree[counter++] = id;
      graphDisjointUnion(disjointSet, cls1, cls2);
    }
  }
  if (counter + 1 != vertNum) {
    // No spanning tree
  }

  graphHeapRelease(heap);
  graphDisjointRelease(disjointSet);
}
