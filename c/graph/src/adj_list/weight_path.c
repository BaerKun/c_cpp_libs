#include "graph/adj_list/weight_path.h"
#include "private/queue.h"

#define HEAP_ELEM_TYPE WeightType *
#define HEAP_LESS_THAN(a, b) (*a < *b)
#include "heap.h"

#include <string.h>

void DijkstraShortest(const Graph *const graph, const WeightType weight[],
                      GraphId predecessor[], const GraphId source,
                      const GraphId target) {
  Heap heap;
  heapInit(&heap, graph->vertNum);
  GraphBool *visited = calloc(graph->vertCap, sizeof(GraphBool));
  WeightType *distance = malloc(graph->vertCap * sizeof(WeightType));
  memset(distance, 0x7f, graph->vertCap * sizeof(WeightType));
  memset(predecessor, 255, graph->vertCap * sizeof(GraphId));

  distance[source] = 0;
  heapPush(&heap, distance + source);
  while (!heapEmpty(&heap)) {
    const GraphId from = (GraphId)(*heapTop(&heap) - distance);
    heapPop(&heap);
    if (from == target) goto END;

    visited[from] = 1;
    for (GraphEdgePtr edge = graph->adjList[from]; edge; edge = edge->next) {
      const GraphId to = edge->to;

      if (visited[to] || distance[to] <= distance[from] + weight[edge->id])
        continue;

      distance[to] = distance[from] + weight[edge->id];
      predecessor[to] = from;
      heapPush(&heap, distance + to);
    }
  }
END:
  free(visited);
  free(distance);
  heapFreeData(&heap);
}

// 无负值圈
void BellmanFordShortest(const Graph *const graph, const WeightType weight[],
                         GraphId predecessor[], const GraphId source) {
  GraphQueue queue;
  graphQueueInit(&queue, graph->vertNum);
  GraphBool *isInQueue = calloc(graph->vertCap, sizeof(GraphBool));
  WeightType *distance = malloc(graph->vertCap * sizeof(WeightType));
  memset(distance, 0x7f, graph->vertCap * sizeof(WeightType));
  memset(predecessor, 255, graph->vertCap * sizeof(GraphId));

  distance[source] = 0;
  isInQueue[source] = 1;
  graphQueuePush(&queue, source);

  while (!graphQueueEmpty(&queue)) {
    const GraphId from = graphQueuePop(&queue);
    isInQueue[from] = 0;

    for (GraphEdgePtr edge = graph->adjList[from]; edge; edge = edge->next) {
      const GraphId to = edge->to;

      if (distance[to] <= distance[from] + weight[edge->id]) continue;

      distance[to] = distance[from] + weight[edge->id];
      predecessor[to] = from;

      if (!isInQueue[to]) {
        graphQueuePush(&queue, to);
        isInQueue[to] = 1;
      }
    }
  }

  free(isInQueue);
  free(distance);
  graphQueueRelease(&queue);
}
