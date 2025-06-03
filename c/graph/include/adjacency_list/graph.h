#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include "share/vertex_edge_data.h"

typedef int VertexId;
typedef struct Vertex_ Vertex, *VertexPtr;
typedef struct Edge_ Edge, *EdgePtr;
typedef struct Graph_ Graph, *GraphPtr;

struct Vertex_ {
  EdgePtr outEdges;
  VertexId path;
  int indegree;
  VertexData data;
};

struct Edge_ {
  EdgePtr next;
  VertexId target;
  int flag;
  EdgePtr reverse;
  EdgeData data;
};

struct Graph_ {
  Vertex *vertices;
  int vertexNum;
  int edgeNum;
  int capacity;
};

void graphInit(GraphPtr graph, int capacity);

void graphDestroy(GraphPtr graph);

static inline void graphAddVertex(const GraphPtr graph, const VertexData data) {
  graph->vertices[graph->vertexNum++] =
      (Vertex){.outEdges = 0, .path = 0, .indegree = 0, .data = data};
}

void graphAddEdge(GraphPtr graph, VertexId source, VertexId target,
                  EdgeData data, int undirected);

#endif // GRAPH_GRAPH_H