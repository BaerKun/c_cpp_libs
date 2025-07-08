#include "private/graph_detail.h"
#include "private/queue.h"
#include "graph/iter.h"

#define HEAP_ELEM_TYPE WeightType *
#define HEAP_LESS_THAN(a, b) (*a < *b)
#include "heap.h"

#include <string.h>

void DijkstraShortest(const Graph *const graph, const WeightType weight[],
                      GraphId predecessor[], const GraphId source,
                      const GraphId target) {
  const GraphView *view = VIEW(graph);
  GraphIter *iter = graphGetIter(graph);
  Heap heap;
  heapInit(&heap, graph->vertNum);
  GraphBool *visited = calloc(view->vertRange, sizeof(GraphBool));
  WeightType *distance = malloc(view->vertRange * sizeof(WeightType));
  memset(distance, UNREACHABLE_BYTE, view->vertRange * sizeof(WeightType));
  memset(predecessor, INVALID_ID, view->vertRange * sizeof(GraphId));

  GraphId id, to;
  distance[source] = 0;
  heapPush(&heap, distance + source);
  while (!heapEmpty(&heap)) {
    const GraphId from = (GraphId)(*heapTop(&heap) - distance);
    heapPop(&heap);
    if (from == target) goto END;

    visited[from] = GRAPH_TRUE;
    while (graphIterNextEdge(iter, from, &id, &to)) {
      if (visited[to] || distance[to] <= distance[from] + weight[id])
        continue;

      distance[to] = distance[from] + weight[id];
      predecessor[to] = from;
      heapPush(&heap, distance + to);
    }
  }
END:
  free(visited);
  free(distance);
  graphIterRelease(iter);
  heapFreeData(&heap);
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
