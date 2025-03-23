#include "adjacency_list/graph.h"
#include <stdlib.h>
#include <stdio.h>
#include "adjacency_list/edge_list.h"

GraphPtr newGraph(const int capacity, const int vertexNum) {
    const GraphPtr graph = malloc(sizeof(Graph));
    graph->capacity = capacity;
    graph->vertexNum = vertexNum;
    graph->edgeNum = 0;
    graph->vertices = malloc(capacity * sizeof(Vertex));

    for(VertexPtr vertex = graph->vertices, end = vertex + graph->vertexNum; vertex != end; ++vertex) {
        vertex->data = NO_VERTEX_DATA;
        vertex->indegree = 0;
        vertex->path = -1;
        vertex->outEdges = NULL;
    }

    return graph;
}

void graphDestroy(const GraphPtr graph) {
    VertexPtr vertex, end;

    for (vertex = graph->vertices, end = vertex + graph->vertexNum; vertex != end; vertex++)
        edgeClear(&vertex->outEdges);

    free(graph->vertices);
    free(graph);
}

void graphAddEdge(const GraphPtr graph, const VertexId source, const VertexId target, const EdgeData data) {
    if (source < 0 || target < 0 || source >= graph->vertexNum || target >= graph->vertexNum) {
        fputs("graphAddEdge:InvalidVertex\n", stderr);
        return;
    }
    if (*edgeFind(&graph->vertices[source].outEdges, target) != NULL)
        return;

    edgeInsertWithData(&graph->vertices[source].outEdges, target, data);
    graph->vertices[target].indegree++;
    graph->edgeNum++;
}

int graphHasPath(const VertexId parent[], const int vertexNum, const VertexId source, VertexId target) {
    if(source < 0 || source >= vertexNum)
        return 0;

    while(source != target){
        if(target < 0 || target >= vertexNum)
            return 0;
        target = parent[target];
    }

    return 1;
}
