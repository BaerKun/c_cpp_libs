#ifndef GRAPH_WEIGHT_PATH_H
#define GRAPH_WEIGHT_PATH_H

#include "graph/graph.h"

void DijkstraShortest(const Graph *graph, const WeightType weight[],
                          GraphId predecessor[], GraphId source,
                          GraphId target);

void BellmanFordShortest(const Graph *graph, const WeightType weight[],
                  GraphId predecessor[], GraphId source);

#endif // GRAPH_WEIGHT_PATH_H
