#ifndef GRAPH_TOPSORT_H
#define GRAPH_TOPSORT_H

#include "graph/graph.h"

void topoSort(const Graph *graph, const GraphInt indegree[], GraphId sort[]);

void topoPath(const Graph *graph, const GraphInt indegree[],
                  GraphId predecessor[]);

#endif // GRAPH_TOPSORT_H
