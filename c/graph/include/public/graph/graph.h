#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include "graph/type.h"

GraphSize graphGetGraphSize();
void graphInit(Graph *graph, GraphBool directed, GraphSize vertCap,
               GraphSize edgeCap);
void graphDestroy(const Graph *graph);

GraphId graphAddVert(Graph *graph);
GraphId graphAddEdge(Graph *graph, GraphId from, GraphId to,
                     GraphBool directed);
void graphReserveVert(Graph *graph, GraphSize num);
void graphDeleteVert(Graph *graph, GraphId id);
void graphDeleteEdge(Graph *graph, GraphId id);

void *graphGetEdgeAttribute(const Graph *graph, const char name[16]);
void graphAddEdgeAttribute(Graph *graph, const char name[16],
                           GraphSize sizeOfElem);

void graphGetIdRange(const Graph *graph, GraphSize *vert, GraphSize *edge);
void graphGetIndegree(const Graph *graph, GraphInt indegree[]);

#endif // GRAPH_GRAPH_H