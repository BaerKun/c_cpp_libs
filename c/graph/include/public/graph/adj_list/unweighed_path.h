#ifndef GRAPH_UNWEIGHED_PATH_H
#define GRAPH_UNWEIGHED_PATH_H

#include "graph/type.h"

void unweightedShortest(const Graph *graph, GraphId predecessor[],
                         GraphId source, GraphId target);

#endif // GRAPH_UNWEIGHED_PATH_H
