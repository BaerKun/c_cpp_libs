#ifndef GRAPH_UNWEIGHED_PATH_H
#define GRAPH_UNWEIGHED_PATH_H

#include "graph.h"

void buildUnweightedPath(const Graph *graph, GraphId predecessor[],
                         GraphId source, GraphId target);

#endif // GRAPH_UNWEIGHED_PATH_H
