#include "graph/linked_path.h"
#include <stdlib.h>

GraphLinkedPath *graphPathInsert(GraphLinkedPath **predNextPtr,
                                 const GraphId id) {
  GraphLinkedPath *const path = malloc(sizeof(GraphLinkedPath));
  path->id = id;
  path->next = *predNextPtr;
  *predNextPtr = path;
  return path;
}

GraphLinkedPath *graphPathUnlink(GraphLinkedPath **predNextPtr) {
  GraphLinkedPath *const path = *predNextPtr;
  *predNextPtr = path->next;
  return path;
}

void graphPathClear(GraphLinkedPath **path) {
  for (GraphLinkedPath *node = *path, *next; node; node = next) {
    next = node->next;
    free(node);
  }
  *path = NULL;
}