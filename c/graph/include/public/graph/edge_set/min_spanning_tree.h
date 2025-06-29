#ifndef GRAPH_EDGE_SET_MIN_SPANNING_TREE_H
#define GRAPH_EDGE_SET_MIN_SPANNING_TREE_H

#include "graph/type.h"

void KruskalMinSpanningTree(const GraphEdgeEndpoint *set,
                            const WeightType weight[], GraphSize edgeNum,
                            GraphSize vertNum, GraphId tree[]);

#endif // GRAPH_EDGE_SET_MIN_SPANNING_TREE_H
