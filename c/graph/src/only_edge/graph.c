#include "only_edge/graph.h"
#include <stdlib.h>
#include <stdio.h>

GraphPtr newGraph(const int edgeCapacity, const int vertexNum) {
    const GraphPtr graph = malloc(sizeof(Graph));

    for (graph->edgeCapacity = 1; edgeCapacity > graph->edgeCapacity; graph->edgeCapacity *= 2);

    graph->edges = malloc(sizeof(Edge) * graph->edgeCapacity);
    graph->edgeNum = 0;
    graph->vertexNum = vertexNum;

    return graph;
}

void graphDestroy(const GraphPtr graph) {
    free(graph->edges);
    free(graph);
}

void graphAddEdge(const GraphPtr graph, const VertexId vertex1, const VertexId vertex2, const EdgeData data) {
    if (graph->edgeNum == graph->edgeCapacity) {
        fputs("graphAddEdge:Graph is full\n", stderr);
        return;
    }

    graph->edges[graph->edgeNum++] = (Edge){vertex1, vertex2, data};
}
