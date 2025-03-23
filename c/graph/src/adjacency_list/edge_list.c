#include "adjacency_list/edge_list.h"
#include <stdlib.h>

void edgeInsertWithData(EdgePtr *const prevNextPtr, const VertexId target, const EdgeData data) {
    const EdgePtr edge = malloc(sizeof(Edge));
    edge->data = data;
    edge->target = target;
    edgeInsert(prevNextPtr, edge);
}

EdgePtr *edgeFind(EdgePtr *const prevNextPtr, const VertexId target) {
    EdgePtr *prev = prevNextPtr;
    for (EdgePtr node = *prev; node && node->target != target; prev = &node->next, node = *prev) continue;
    return prev;
}

EdgePtr edgeUnlinkWithTarget(EdgePtr *const prevNextPtr, const VertexId target) {
    EdgePtr *const prev = edgeFind(prevNextPtr, target);
    return edgeUnlink(prev);
}

void edgeClear(EdgePtr *const prevNextPtr) {
    for (EdgePtr node = *prevNextPtr; node; node = *prevNextPtr) {
        *prevNextPtr = node->next;
        free(node);
    }
}