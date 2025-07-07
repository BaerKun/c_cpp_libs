#ifndef GRAPH_LINKED_PATH_H
#define GRAPH_LINKED_PATH_H

#include "graph/type.h"

GraphLinkedNode *graphPathInsert(GraphLinkedNode **predNextPtr, GraphId id);

GraphLinkedNode *graphPathUnlink(GraphLinkedNode **predNextPtr);

void graphPathClear(GraphLinkedNode **path);

#endif // GRAPH_LINKED_PATH_H
