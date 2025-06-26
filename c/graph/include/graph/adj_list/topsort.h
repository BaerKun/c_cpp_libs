#ifndef GRAPH_TOPSORT_H
#define GRAPH_TOPSORT_H

#include "graph.h"

void topoSort(const Graph *graph, const GraphInt indegree[], GraphId sort[]);

void buildTopPath(const Graph *graph, const GraphInt indegree[],
                  GraphId predecessor[]);

#endif // GRAPH_TOPSORT_H
