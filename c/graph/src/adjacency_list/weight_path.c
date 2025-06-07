#include "adjacency_list/weight_path.h"
#include "queue.h"

#define HEAP_DATA_TYPE int *
#define HEAP_LESS_THAN(a, b) (*a < *b)
#include "heap.h"

void DijkstraWeightedPath(const ListGraphPtr graph, VertexId *parent, const VertexId source,
                          const VertexId target) {
  char *hasKnown = malloc(graph->vertNum);
  int *distance = malloc(graph->vertNum * sizeof(int));
  Heap heap;
  heapInit(&heap, graph->vertNum);

  for (VertexId vertex = 0; vertex < graph->vertNum; vertex++) {
    hasKnown[vertex] = 0;
    distance[vertex] = UNREACHABLE;
  }

  distance[source] = 0;
  heapPush(&heap, distance + source);

  while (heap.size) {
    const VertexId vertex = (VertexId)(*heapTop(&heap) - distance);
    heapPop(&heap);
    if (vertex == target) return;

    hasKnown[vertex] = 1;
    for (ListEdgePtr edge = graph->vertices[vertex].outEdges; edge; edge = edge->next) {
      const VertexId adjacentVertex = edge->target;

      if (hasKnown[adjacentVertex] ||
          distance[adjacentVertex] <= distance[vertex] + edge->tail->weight)
        continue;

      distance[adjacentVertex] = distance[vertex] + edge->tail->weight;
      parent[adjacentVertex] = vertex;
      heapPush(&heap, distance + adjacentVertex);
    }
  }

  free(hasKnown);
  free(distance);
  heapFreeData(&heap);
}

// 无负值圈
void weightedPath(const ListGraphPtr graph, VertexId *parent, const VertexId source) {
  char *isInQueue = malloc(graph->vertNum);
  int *distance = malloc(graph->vertNum * sizeof(int));
  Queue queue;
  queueInit(&queue, graph->vertNum);

  for (VertexId vertex = 0; vertex < graph->vertNum; vertex++) {
    isInQueue[vertex] = 0;
    distance[vertex] = UNREACHABLE;
  }

  enqueue(&queue, source);
  distance[source] = 0;
  isInQueue[source] = 1;

  while (queue.size) {
    const VertexId vertex = *queueFront(&queue);
    dequeue(&queue);
    isInQueue[vertex] = 0;

    for (ListEdgePtr edge = graph->vertices[vertex].outEdges; edge; edge = edge->next) {
      const VertexId adjacentVertex = edge->target;

      if (distance[adjacentVertex] <= distance[vertex] + edge->tail->weight) continue;

      distance[adjacentVertex] = distance[vertex] + edge->tail->weight;
      parent[adjacentVertex] = vertex;

      if (!isInQueue[adjacentVertex]) {
        enqueue(&queue, adjacentVertex);
        isInQueue[adjacentVertex] = 1;
      }
    }
  }

  free(isInQueue);
  free(distance);
  queueFreeData(&queue);
}
