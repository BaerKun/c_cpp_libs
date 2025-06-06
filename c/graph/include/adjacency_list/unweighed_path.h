#ifndef GRAPH_UNWEIGHED_PATH_H
#define GRAPH_UNWEIGHED_PATH_H

#include "graph.h"

void buildUnweightedPath(ListGraphPtr graph, VertexId *parent, VertexId source,
                         VertexId target);

#endif // GRAPH_UNWEIGHED_PATH_H
