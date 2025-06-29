#ifndef MAX_FLOW_H
#define MAX_FLOW_H

#include "graph/type.h"
typedef WeightType FlowType;

FlowType EdmondsKarpMaxFlow(const Graph *network, const FlowType capacity[],
                            FlowType flow[], GraphId source, GraphId sink);

#endif // MAX_FLOW_H
