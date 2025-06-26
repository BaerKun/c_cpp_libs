#include "graph/adj_list/weight_path.h"
#include "queue.h"

#define HEAP_DATA_TYPE WeightType *
#define HEAP_LESS_THAN(a, b) (*a < *b)
#include "heap.h"

#include <string.h>

void DijkstraWeightedPath(const Graph *const graph, const WeightType weight[],
                          GraphId predecessor[], const GraphId source,
                          const GraphId target) {
  Heap heap;
  heapInit(&heap, graph->vertNum);
  GraphBool *visited = calloc(graph->vertCap, sizeof(GraphBool));
  WeightType *distance = malloc(graph->vertCap * sizeof(WeightType));
  memset(distance, 0x7f, graph->vertCap * sizeof(WeightType));

  distance[source] = 0;
  heapPush(&heap, distance + source);
  while (heap.size) {
    const GraphId from = (GraphId)(*heapTop(&heap) - distance);
    heapPop(&heap);
    if (from == target) return;

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

  free(visited);
  free(distance);
  heapFreeData(&heap);
}

// 无负值圈
void weightedPath(const Graph *const graph, const WeightType weight[],
                  GraphId predecessor[], const GraphId source) {
  Queue queue;
  queueInit(&queue, graph->vertNum);
  GraphBool *isInQueue = calloc(graph->vertCap, sizeof(GraphBool));
  WeightType *distance = malloc(graph->vertCap * sizeof(WeightType));
  memset(distance, 0x7f, graph->vertCap * sizeof(WeightType));

  enqueue(&queue, source);
  distance[source] = 0;
  isInQueue[source] = 1;

  while (queue.size) {
    const GraphId from = *queueFront(&queue);
    dequeue(&queue);
    isInQueue[from] = 0;

    for (GraphEdgePtr edge = graph->adjList[from]; edge; edge = edge->next) {
      const GraphId to = edge->to;

      if (distance[to] <= distance[from] + weight[edge->id]) continue;

      distance[to] = distance[from] + weight[edge->id];
      predecessor[to] = from;

      if (!isInQueue[to]) {
        enqueue(&queue, to);
        isInQueue[to] = 1;
      }
    }
  }

  free(isInQueue);
  free(distance);
  queueFreeData(&queue);
}
