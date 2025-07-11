#include "graph/linked_list.h"
#include <stdlib.h>

GraphLinkedNode *graphLinkedInsert(GraphLinkedNode **predNextPtr,
                                 const GraphId id) {
  GraphLinkedNode *const path = malloc(sizeof(GraphLinkedNode));
  path->id = id;
  path->next = *predNextPtr;
  *predNextPtr = path;
  return path;
}

GraphLinkedNode *graphLinkedUnlink(GraphLinkedNode **predNextPtr) {
  GraphLinkedNode *const path = *predNextPtr;
  *predNextPtr = path->next;
  return path;
}

void graphLinkedClear(GraphLinkedNode **path) {
  for (GraphLinkedNode *node = *path, *next; node; node = next) {
    next = node->next;
    free(node);
  }
  *path = NULL;
}