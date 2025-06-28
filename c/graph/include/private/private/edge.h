#ifndef EDGE_LIST_H
#define EDGE_LIST_H

#include "graph/graph.h"

static inline void edgeInsert(GraphEdgePtr *const prevNextPtr, const GraphEdgePtr edge) {
  edge->next = *prevNextPtr;
  *prevNextPtr = edge;
}

static inline GraphEdgePtr edgeUnlink(GraphEdgePtr *const prevNextPtr) {
  const GraphEdgePtr edge = *prevNextPtr;
  *prevNextPtr = edge->next;
  return edge;
}

#endif // EDGE_LIST_H
