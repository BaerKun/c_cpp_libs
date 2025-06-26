#ifndef MAX_FLOW_H
#define MAX_FLOW_H

#include "graph/adj_list/graph.h"
typedef WeightType FlowType;

FlowType EdmondsKarpMaxFlow(const Graph *network, const FlowType capacity[],
                            GraphId source, GraphId sink);

#endif // MAX_FLOW_H
