#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include "share/type.h"

typedef struct ListVertex_ ListVertex, *ListVertexPtr;
typedef struct ListEdge_ ListEdge, *ListEdgePtr;
typedef struct ListGraph_ ListGraph, *ListGraphPtr;

struct ListVertex_ {
  int indegree;
  ListEdgePtr outEdges;
  VertexData data;
};

struct ListEdge_ {
  int flag;
  VertexId target;
  WeightType weight;
  ListEdgePtr reverse;
  ListEdgePtr next;
  EdgeData data;
};

struct ListGraph_ {
  ListVertex *vertices;
  int vertexNum;
  int edgeNum;
  int capacity;
};

void listGraphInit(ListGraphPtr graph, int capacity);

void listGraphDestroy(ListGraphPtr graph);

static inline void listGraphAddVertex(const ListGraphPtr graph, const VertexData data) {
  graph->vertices[graph->vertexNum++] = (ListVertex){.outEdges = 0, .indegree = 0, .data = data};
}

void listGraphAddEdge(ListGraphPtr graph, VertexId source, VertexId target, WeightType weight,
                      VertexData data, int directed);

#endif // GRAPH_GRAPH_H