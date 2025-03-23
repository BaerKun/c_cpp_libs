#include "adjacency_list/weight_path.h"
#include "queue.h"

#define HEAP_ELEMENT_TYPE int *
#define HEAP_LESS_THAN(a, b) (*a < *b)
#include "heap.h"

#ifdef WEIGHT_FOR_WEIGHTED_PATH

void DijkstraWeightedPath(const GraphPtr graph, VertexId *parent, const VertexId source, const VertexId target) {
    char *hasKnown = malloc(graph->vertexNum);
    int *distance = malloc(graph->vertexNum * sizeof(int));
    const HeapPtr heap = newHeap(graph->vertexNum);

    for (VertexId vertex = 0; vertex < graph->vertexNum; vertex++) {
        hasKnown[vertex] = 0;
        distance[vertex] = WIGHT_MAX;
    }

    distance[source] = 0;
    heapInsert(heap, distance + source);

    while (heap->size) {
        const VertexId vertex = (VertexId) (heapDeleteMin(heap) - distance);
        if (vertex == target)
            return;

        hasKnown[vertex] = 1;
        for (EdgePtr edge = graph->vertices[vertex].outEdges; edge; edge = edge->next) {
            const VertexId adjacentVertex = edge->target;

            if (hasKnown[adjacentVertex] || distance[adjacentVertex] <= distance[vertex] + edge->data.WEIGHT_FOR_WEIGHTED_PATH)
                continue;

            distance[adjacentVertex] = distance[vertex] + edge->data.WEIGHT_FOR_WEIGHTED_PATH;
            parent[adjacentVertex] = vertex;
            heapInsert(heap, distance + adjacentVertex);
        }
    }

    free(hasKnown);
    free(distance);
    heapDestroy(heap);
}

// 无负值圈
void weightedPath(const GraphPtr graph, VertexId *parent, const VertexId source) {
    char *isInQueue = malloc(graph->vertexNum);
    int *distance = malloc(graph->vertexNum * sizeof(int));
    const QueuePtr queue = newQueue(graph->vertexNum);

    for (VertexId vertex = 0; vertex < graph->vertexNum; vertex++) {
        isInQueue[vertex] = 0;
        distance[vertex] = WIGHT_MAX;
    }

    enqueue(queue, source);
    distance[source] = 0;
    isInQueue[source] = 1;

    while (queue->front != queue->rear) {
        const VertexId vertex = dequeue(queue);
        isInQueue[vertex] = 0;

        for (EdgePtr edge = graph->vertices[vertex].outEdges; edge; edge = edge->next) {
            const VertexId adjacentVertex = edge->target;

            if (distance[adjacentVertex] <= distance[vertex] + edge->data.WEIGHT_FOR_WEIGHTED_PATH)
                continue;

            distance[adjacentVertex] = distance[vertex] + edge->data.WEIGHT_FOR_WEIGHTED_PATH;
            parent[adjacentVertex] = vertex;

            if (!isInQueue[adjacentVertex]) {
                enqueue(queue, adjacentVertex);
                isInQueue[adjacentVertex] = 1;
            }
        }
    }

    free(isInQueue);
    free(distance);
    queue_destroy(queue);
}

#endif