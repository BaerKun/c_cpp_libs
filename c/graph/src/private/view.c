#include "private/view.h"
#include <stdlib.h>
#include <string.h>

#define DID(eid) ((eid) << 1 | (eid) >> (sizeof(GraphId) * 8 - 1))

GraphIter *graphIterFromView(const GraphView *view) {
  GraphIter *iter =
      malloc(sizeof(GraphIter) + view->vertRange * sizeof(GraphId));
  iter->view = view;
  iter->vertCurr = view->vertHead;
  memcpy(iter->edgeCurr, view->edgeHead, view->vertRange * sizeof(GraphId));
  return iter;
}

GraphView *graphViewReserveEdge(const GraphView *view,
                                const GraphBool directed) {
  void *buff = malloc(sizeof(GraphView) +
                      (view->vertRange + (directed ? 1 : 2) * view->edgeRange) *
                          sizeof(GraphId));
  GraphView *copy = buff;
  copy->vertRange = view->vertRange;
  copy->vertHead = view->vertHead;
  copy->vertNext = view->vertNext;

  copy->directed = directed;
  copy->edgeRange = view->edgeRange;
  copy->edgeHead = buff + sizeof(GraphView);
  copy->edgeNext = copy->edgeHead + view->vertRange;
  copy->endpts = view->endpts;
  return copy;
}

void graphViewCopyEdge(const GraphView *view, const GraphView *copy) {
  switch (view->directed << 1 | copy->directed) {
  case 3:
    memcpy(copy->edgeHead, view->edgeHead, copy->vertRange * sizeof(GraphId));
    memcpy(copy->edgeNext, view->edgeNext, copy->edgeRange * sizeof(GraphId));
    break;
  case 0:
    memcpy(copy->edgeHead, view->edgeHead, copy->vertRange * sizeof(GraphId));
    memcpy(copy->edgeNext, view->edgeNext,
           2 * copy->edgeRange * sizeof(GraphId));
    break;
  case 2:
    for (GraphSize i = 0; i < copy->vertRange; ++i)
      copy->edgeHead[i] = DID(view->edgeHead[i]);
    for (GraphSize i = 0; i < copy->edgeRange; ++i)
      copy->edgeNext[i << 1] = DID(view->edgeNext[i]);
    break;
  default:;
  }
}

GraphId *graphFind(GraphId *next, GraphId *head, const GraphId id) {
  GraphId *predNext = head;
  while (*predNext != INVALID_ID && *predNext != id)
    predNext = next + *predNext;
  return *predNext == INVALID_ID ? 0 : predNext;
}
