#ifndef GRAPH_MIN_SPANNING_TREE_H
#define GRAPH_MIN_SPANNING_TREE_H

#include "graph/type.h"

void KruskalMinSpanningTree(const GraphEdgeEndpoint *set,
                            const WeightType weight[], GraphSize edgeNum,
                            GraphSize vertNum, GraphId tree[]);

#endif // GRAPH_MIN_SPANNING_TREE_H
