#include "adjacency_list/unweighed_path.h"
#include "queue.h"

void buildUnweightedPath(const GraphPtr graph, VertexId *parent, const VertexId source, const VertexId target) {
    VertexId vertex;
    Queue queue;
    queueInit(&queue, graph->vertexNum);

    for(vertex = 0; vertex < graph->vertexNum; vertex++)
        parent[vertex] = -1;

    enqueue(&queue, source);
    while (queue.size) {
        vertex = dequeue(&queue);

        for (EdgePtr edge = graph->vertices[vertex].outEdges; edge; edge = edge->next) {
            const VertexId adjacentVertex = edge->target;

            if (parent[adjacentVertex] == -1) {
                parent[adjacentVertex] = vertex;
                if (adjacentVertex == target)
                    return;

                enqueue(&queue, adjacentVertex);
            }
        }
    }
    queueFreeData(&queue);
}