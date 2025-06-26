#ifndef GRAPH_WEIGHT_PATH_H
#define GRAPH_WEIGHT_PATH_H

#include "graph.h"

void DijkstraWeightedPath(const Graph *graph, const WeightType weight[],
                          GraphId predecessor[], GraphId source,
                          GraphId target);

void weightedPath(const Graph *graph, const WeightType weight[],
                  GraphId predecessor[], GraphId source);

#endif // GRAPH_WEIGHT_PATH_H
