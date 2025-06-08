#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include "share/type.h"

enum { UNWEIGHTED = 0, WEIGHTED, FLOW_NETWORK, ANA };

typedef int FlowType;
typedef int TimeType;
typedef struct ListVertex_ ListVertex, *ListVertexPtr;
typedef struct ListEdge_ ListEdge, *ListEdgePtr;
typedef struct ListGraph_ ListGraph, *ListGraphPtr;

typedef struct {
  FlowType capacity, cost, current;
} FlowTail;
typedef struct {
  TimeType duration, earlyStart, lateStart, slack;
} AnaTail;
typedef union {
  WeightType weight;
  FlowTail flow;
  AnaTail ana;
} EdgeTail;

struct ListVertex_ {
  int indegree;
  ListEdgePtr outEdges;
  VertexData userdata;
};

struct ListEdge_ {
  int flag;
  VertexId target;
  ListEdgePtr reverse;
  ListEdgePtr next;
  EdgeData userdata;
  EdgeTail tail[0];
};

struct ListGraph_ {
  int type;
  int edgeNum;
  int vertCap;
  int vertNum;
  ListVertex *vertices;
};

void listGraphInit(ListGraphPtr graph, int type, int vertCap);

void listGraphDestroy(ListGraphPtr graph);

void listGraphAddVertex(ListGraphPtr graph, VertexData data);

void listGraphReserve(ListGraphPtr graph, int vertNum);

void listGraphAddEdge(ListGraphPtr graph, VertexId source, VertexId target, EdgeData data,
                      const EdgeTail *tail, int directed);

#endif // GRAPH_GRAPH_H