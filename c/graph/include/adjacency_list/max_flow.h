#ifndef MAX_FLOW_H
#define MAX_FLOW_H

#include "adjacency_list/graph.h"

FlowType EdmondsKarpMaxFlow(const ListGraph *network, VertexId source, VertexId sink);

#endif // MAX_FLOW_H
