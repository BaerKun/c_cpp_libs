#ifndef GRAPH_MIN_SPANNING_TREE_H
#define GRAPH_MIN_SPANNING_TREE_H

#include "edge_set.h"

void KruskalMinSpanningTree(const GraphEdgeSetElem *set,
                            const WeightType weight[], GraphSize edgeNum,
                            GraphSize vertNum, GraphId tree[]);

#endif // GRAPH_MIN_SPANNING_TREE_H
