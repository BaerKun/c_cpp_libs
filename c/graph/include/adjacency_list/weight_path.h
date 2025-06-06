#ifndef GRAPH_WEIGHT_PATH_H
#define GRAPH_WEIGHT_PATH_H

#include "graph.h"

void DijkstraWeightedPath(ListGraphPtr graph, VertexId *parent, VertexId source, VertexId target);

void weightedPath(ListGraphPtr graph, VertexId *parent, VertexId source);

#endif // GRAPH_WEIGHT_PATH_H
