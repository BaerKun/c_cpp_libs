#include "matrix/graph.h"
#include <stdlib.h>

#define INIT_EDGE (Edge){0x7fffffff, 0}

GraphPtr createGraph(const int capacity, const int vertexNum) {
    const GraphPtr graph = (GraphPtr) malloc(sizeof(Graph));
    if (graph == NULL)
        return NULL;

    graph->capacity = capacity;
    graph->edgeNum = 0;
    graph->vertexNum = vertexNum;
    graph->vertices = (Vertex *) malloc(sizeof(Vertex) * capacity);
    graph->edges = (Edge (*)[MAX_VERTEX]) malloc(sizeof(Edge) * capacity * MAX_VERTEX);

    if (graph->edges == NULL) {
        free(graph);
        return NULL;
    }

    for (VertexId source = 0; source < graph->vertexNum; source++)
        for (VertexId target = 0; target < graph->vertexNum; target++)
            graph->edges[source][target] = INIT_EDGE;

    return graph;
}

void deleteGraph(const GraphPtr graph) {
    free(graph->vertices);
    free(graph->edges);
    free(graph);
}

void addEdge(const GraphPtr graph, const VertexId source, const VertexId target, const EdgeData data) {
    graph->edges[source][target] = (Edge){data, target};
    graph->edgeNum++;
}
