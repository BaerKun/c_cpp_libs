#include "private/view.h"
#include <stdlib.h>
#include <string.h>

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
  copy->vertNext = copy->vertNext;

  copy->directed = directed;
  copy->edgeRange = view->edgeRange;
  copy->edgeHead = buff + sizeof(GraphView);
  copy->edgeNext = copy->edgeHead + view->vertRange;
  copy->endpts = view->endpts;
  return copy;
}

void graphViewCopyEdge(const GraphView *view, const GraphView *copy) {
  memcpy(copy->edgeHead, view->edgeHead, view->vertRange * sizeof(GraphId));
  if (view->directed == copy->directed) {
    memcpy(copy->edgeNext, view->edgeNext,
           (copy->directed ? 1 : 2) * view->vertRange * sizeof(GraphId));
  } else {
    for (GraphSize i = 0; i < copy->vertRange; i++) {
      copy->edgeNext[i << 1] = view->edgeNext[i] << 1 | view->edgeNext[i] < 0;
    }
  }
}

GraphId *graphFind(GraphId *next, GraphId *head, const GraphId id) {
  GraphId *predNext = head;
  while (*predNext != INVALID_ID && *predNext != id)
    predNext = next + *predNext;
  return *predNext == INVALID_ID ? 0 : predNext;
}
