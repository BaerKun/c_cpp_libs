#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include "share/vertex_edge.h"

typedef int VertexId;
typedef struct Vertex_ Vertex, *VertexPtr;
typedef struct Edge_ Edge, *EdgePtr;
typedef struct Graph_ Graph, *GraphPtr;

struct Vertex_ {
    VertexData data;
    int indegree;
    VertexId path;
    EdgePtr outEdges;
};

struct Edge_ {
    EdgePtr next;
    VertexId target;
    EdgeData data;
};

struct Graph_ {
    int capacity;
    int vertexNum;
    int edgeNum;
    VertexPtr vertices;
};

GraphPtr newGraph(int capacity, int vertexNum);

void graphDestroy(GraphPtr graph);

void graphAddEdge(GraphPtr graph, VertexId source, VertexId target, EdgeData data);

int graphHasPath(const VertexId parent[], int vertexNum, VertexId source, VertexId target);

#endif //GRAPH_GRAPH_H
