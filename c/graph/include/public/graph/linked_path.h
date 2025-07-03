#ifndef GRAPH_LINKED_PATH_H
#define GRAPH_LINKED_PATH_H

#include "graph/type.h"

GraphLinkedPath *graphPathInsert(GraphLinkedPath **predNextPtr, GraphId id);

GraphLinkedPath *graphPathUnlink(GraphLinkedPath **predNextPtr);

void graphPathClear(GraphLinkedPath **path);

#endif // GRAPH_LINKED_PATH_H
