#ifndef GRAPH_MIN_SPANNING_TREE_H
#define GRAPH_MIN_SPANNING_TREE_H

#include "graph/type.h"

void PrimMinSpanningTree(const Graph *graph, const WeightType weight[],
                         GraphId predecessor[], GraphId root);

#endif // GRAPH_MIN_SPANNING_TREE_H
