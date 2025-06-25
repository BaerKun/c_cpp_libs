#ifndef MAX_FLOW_H
#define MAX_FLOW_H

#include "adj_list/graph.h"

FlowType EdmondsKarpMaxFlow(const Graph *network, VertexId source, VertexId sink);

#endif // MAX_FLOW_H
