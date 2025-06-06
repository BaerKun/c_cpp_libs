#ifndef EDGE_LIST_H
#define EDGE_LIST_H

#include "graph.h"

static inline void edgeInsert(ListEdgePtr *const prevNextPtr, const ListEdgePtr edge) {
  edge->next = *prevNextPtr;
  *prevNextPtr = edge;
}

static inline ListEdgePtr edgeUnlink(ListEdgePtr *const prevNextPtr) {
  const ListEdgePtr edge = *prevNextPtr;
  *prevNextPtr = edge->next;
  return edge;
}

#endif // EDGE_LIST_H
