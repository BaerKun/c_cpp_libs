#include "graph/iter.h"
#include "private/graph_detail.h"
#include "private/heap.h"
#include "private/queue.h"
#include <stdlib.h>
#include <string.h>

void DijkstraShortest(const Graph *const graph, const WeightType weight[],
                      GraphId predecessor[], const GraphId source,
                      const GraphId target) {
  const GraphView *view = VIEW(graph);
  GraphIter *iter = graphGetIter(graph);
  GraphBool *visited = calloc(view->vertRange, sizeof(GraphBool));
  WeightType *distance = malloc(view->vertRange * sizeof(WeightType));
  GraphHeap *heap = graphHeapCreate(graph->vertNum, distance);
  memset(distance, UNREACHABLE_BYTE, view->vertRange * sizeof(WeightType));
  memset(predecessor, INVALID_ID, view->vertRange * sizeof(GraphId));

  GraphId id, to;
  distance[source] = 0;
  graphHeapPush(heap, source);
  while (!graphHeapEmpty(heap)) {
    const GraphId from = graphHeapPop(heap);
    if (from == target) break;

    visited[from] = GRAPH_TRUE;
    while (graphIterNextEdge(iter, from, &id, &to)) {
      if (visited[to] || distance[to] <= distance[from] + weight[id]) continue;

      distance[to] = distance[from] + weight[id];
      predecessor[to] = from;
      graphHeapPush(heap, to);
    }
  }

  free(visited);
  free(distance);
  graphIterRelease(iter);
  graphHeapRelease(heap);
}

// 无负值圈
void BellmanFordShortest(const Graph *const graph, const WeightType weight[],
                         GraphId predecessor[], const GraphId source) {
  const GraphView *view = VIEW(graph);
  GraphIter *iter = graphGetIter(graph);
  GraphQueue *queue = graphNewQueue(graph->vertNum);
  GraphBool *isInQueue = calloc(view->vertRange, sizeof(GraphBool));
  WeightType *distance = malloc(view->vertRange * sizeof(WeightType));
  memset(distance, UNREACHABLE_BYTE, view->vertRange * sizeof(WeightType));
  memset(predecessor, INVALID_ID, view->vertRange * sizeof(GraphId));

  GraphId id, to;
  distance[source] = 0;
  isInQueue[source] = GRAPH_TRUE;
  graphQueuePush(queue, source);
  while (!graphQueueEmpty(queue)) {
    const GraphId from = graphQueuePop(queue);
    isInQueue[from] = GRAPH_TRUE;

    while (graphIterNextEdge(iter, from, &id, &to)) {
      if (distance[to] <= distance[from] + weight[id]) continue;

      distance[to] = distance[from] + weight[id];
      predecessor[to] = from;

      if (!isInQueue[to]) {
        graphQueuePush(queue, to);
        graphIterResetEdge(iter, to);
        isInQueue[to] = GRAPH_TRUE;
      }
    }
  }

  free(isInQueue);
  free(distance);
  graphIterRelease(iter);
  graphQueueRelease(queue);
}
