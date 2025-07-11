#ifndef GRAPH_LINKED_PATH_H
#define GRAPH_LINKED_PATH_H

#include "graph/type.h"

GraphLinkedNode *graphLinkedInsert(GraphLinkedNode **predNextPtr, GraphId id);

GraphLinkedNode *graphLinkedUnlink(GraphLinkedNode **predNextPtr);

void graphLinkedClear(GraphLinkedNode **path);

#endif // GRAPH_LINKED_PATH_H
