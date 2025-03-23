#include "share/init_indegree.h"

void InitIndegree(const GraphPtr graph, int *indegree, const QueuePtr queue) {
    VertexPtr vertex = graph->vertices;
    const VertexPtr end = vertex + graph->vertexNum;
    int *dst = indegree;

    while (vertex != end) {
        *dst = vertex->indegree;
        if (*dst == 0)
            enqueue(queue, vertex - graph->vertices);

        ++dst;
        ++vertex;
    }
}
