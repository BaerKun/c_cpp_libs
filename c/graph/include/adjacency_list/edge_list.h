#ifndef EDGE_LIST_H
#define EDGE_LIST_H

#include "graph.h"

static inline void edgeInsert(EdgePtr *const prevNextPtr, const EdgePtr edge) {
    edge->next = *prevNextPtr;
    *prevNextPtr = edge;
}

static inline EdgePtr edgeUnlink(EdgePtr *const prevNextPtr) {
    const EdgePtr edge = *prevNextPtr;
    *prevNextPtr = edge->next;
    return edge;
}

void edgeInsertWithData(EdgePtr *prevNextPtr, VertexId target, EdgeData data);

EdgePtr edgeUnlinkWithTarget(EdgePtr *prevNextPtr, VertexId target);

EdgePtr *edgeFind(EdgePtr *prevNextPtr, VertexId target);

void edgeClear(EdgePtr *prevNextPtr);

#endif //EDGE_LIST_H
