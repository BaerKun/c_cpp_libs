#ifndef GRAPH_TOPSORT_H
#define GRAPH_TOPSORT_H

#include "graph.h"

void topSort(GraphPtr graph, VertexId sortArray[]);

void buildTopPath(GraphPtr graph, VertexId parent[]);

#endif //GRAPH_TOPSORT_H
