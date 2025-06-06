#ifndef NETWORK_H
#define NETWORK_H

#include "share/type.h"

typedef WeightType FlowType;
typedef struct NetworkVertex_ NetworkVertex, *NetworkVertexPtr;
typedef struct NetworkEdge_ NetworkEdge, *NetworkEdgePtr;
typedef struct Network_ Network, *NetworkPtr;

struct NetworkVertex_ {
  NetworkEdgePtr outEdges;
  VertexData data;
};

struct NetworkEdge_ {
  VertexId target;
  FlowType capacity, flow;
  NetworkEdgePtr next;
  EdgeData data;
};

struct Network_ {
  int vertexNum;
  int edgeNum;
  NetworkVertex *vertices;
};

void networkInit(NetworkPtr network, int vertexNum);

void networkAddEdge(NetworkPtr network, VertexId source, VertexId target,
                   FlowType capacity, EdgeData data);

void networkDestroy(NetworkPtr network);

#endif // NETWORK_H
