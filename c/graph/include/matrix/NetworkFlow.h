#ifndef NETWORKFLOW_H
#define NETWORKFLOW_H

typedef int FlowType;
#define FLOW_MAX 0x7fffffff

#define EDGE_DATA

struct EdgeData {
    FlowType capacity;
    FlowType flow;
};

#include "graph.h"

typedef Graph Network, *NetworkPtr;

NetworkPtr CopyNetwork(NetworkPtr network);

FlowType EdmondKarpMaxFlow(NetworkPtr network, VertexId source, VertexId sink);

#endif //NETWORKFLOW_H
