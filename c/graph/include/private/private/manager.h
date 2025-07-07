#ifndef MANAGER_H
#define MANAGER_H

#include "graph_detail.h"

void graphManagerInit(GraphManager *mgr, GraphBool directed, GraphSize vertCap,
                      GraphSize edgeCap);
void graphManagerDestroy(const GraphManager *mgr);

GraphId graphManagerNewVert(GraphManager *mgr);
GraphId graphManagerNewEdge(GraphManager *mgr, GraphId from, GraphId to,
                            GraphBool directed);

void graphManagerDeleteVert(GraphManager *mgr, GraphId id);
void graphManagerDeleteEdge(GraphManager *mgr, GraphId id);

#endif // MANAGER_H
