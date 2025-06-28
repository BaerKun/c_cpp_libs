#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include "graph/type.h"

void graphInit(Graph *graph, GraphSize vertCap, GraphSize edgeCap);

void graphDestroy(const Graph *graph);

GraphId graphAddVertex(Graph *graph);

GraphId graphAddEdge(Graph *graph, GraphId from, GraphId to,
                     GraphBool directed);

void graphGetIndegree(const Graph *graph, GraphInt indegree[]);

void graphAddEdgeAttribute(Graph *graph, const char name[16],
                           GraphSize sizeOfElem);

void *graphGetEdgeAttribute(const Graph *graph, const char name[16]);

#endif // GRAPH_GRAPH_H