#ifndef MAX_FLOW_H
#define MAX_FLOW_H

#include "network.h"

FlowType EdmondKarpMaxFlow(const Network *network, VertexId source, VertexId sink);

#endif // MAX_FLOW_H
