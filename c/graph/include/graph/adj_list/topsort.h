#ifndef GRAPH_TOPSORT_H
#define GRAPH_TOPSORT_H

#include "graph.h"

void topSort(ListGraphPtr graph, VertexId sortArray[]);

void buildTopPath(ListGraphPtr graph, VertexId parent[]);

#endif // GRAPH_TOPSORT_H
