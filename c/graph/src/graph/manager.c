#include "private/manager.h"
#include "private/utils.h"
#include <stdlib.h>
#include <string.h>

void graphManagerInit(GraphManager *mgr, const GraphBool directed,
                      const GraphSize vertCap, const GraphSize edgeCap) {
  mgr->vertFree = mgr->edgeFree = 0;
  GraphView *const view = &mgr->view;
  view->directed = directed;
  view->vertRange = view->edgeRange = 0;
  view->endpts = malloc(edgeCap * sizeof(GraphEndpoint));
  view->vertHead = INVALID_ID;
  view->edgeHead = malloc(vertCap * sizeof(GraphId));
  memset(view->edgeHead, INVALID_ID, vertCap * sizeof(GraphId));
  if (directed) {
    mgr->buff = view->edgeNext = malloc(edgeCap * sizeof(GraphId));
  } else {
    mgr->buff = malloc(2 * edgeCap * sizeof(GraphId));
    view->edgeNext = mgr->buff + edgeCap;
  }
  view->vertNext = malloc(vertCap * sizeof(GraphId));
  view->vertNext[vertCap - 1] = view->edgeNext[edgeCap - 1] = INVALID_ID;
  for (GraphId i = (GraphId)vertCap - 1; i; --i) view->vertNext[i - 1] = i;
  for (GraphId i = (GraphId)edgeCap - 1; i; --i) view->edgeNext[i - 1] = i;
}

void graphManagerDestroy(const GraphManager *mgr) {
  free(mgr->view.edgeHead);
  free(mgr->view.vertNext);
  free(mgr->buff);
}

GraphId graphManagerNewVert(GraphManager *mgr) {
  GraphView *view = &mgr->view;
  const GraphId id = mgr->vertFree;
  graphUnlink(view->vertNext, &mgr->vertFree);
  graphInsert(view->vertNext, &view->vertHead, id);
  if (id == view->vertRange) ++view->vertRange;
  return id;
}

GraphId graphManagerNewEdge(GraphManager *mgr, const GraphId from,
                            const GraphId to, const GraphBool directed) {
  GraphView *view = &mgr->view;
  const GraphId id = mgr->edgeFree;
  view->endpts[id] = (GraphEndpoint){from, to};
  graphUnlink(view->edgeNext, &mgr->edgeFree);
  graphInsert(view->edgeNext, view->edgeHead + from, id);
  if (!directed) graphInsert(view->edgeNext, view->edgeHead + to, REVERSE(id));
  if (id == view->edgeRange) ++view->edgeRange;
  return id;
}

void graphManagerDeleteVert(GraphManager *mgr, const GraphId id) {
  GraphView *view = &mgr->view;
  GraphId *predNext = graphFind(view->vertNext, &view->vertHead, id);
  graphUnlink(view->vertNext, predNext);
  graphInsert(view->vertNext, &mgr->vertFree, id);
  if (id == view->vertRange - 1) view->vertRange = id;
}

void graphManagerDeleteEdge(GraphManager *mgr, const GraphId id) {
  GraphView *view = &mgr->view;
  const GraphId from = view->endpts[id].from;
  const GraphId to = view->endpts[id].to;
  GraphId *predNext = graphFind(view->edgeNext, view->edgeHead + from, id);
  graphUnlink(view->edgeNext, predNext);
  graphInsert(view->edgeNext, &mgr->edgeFree, id);
  if (id == view->edgeRange - 1) view->edgeRange = id;
  if (!view->directed) {
    predNext = graphFind(view->edgeNext, view->edgeHead + to, REVERSE(id));
    if (predNext) graphUnlink(view->edgeNext, predNext);
  }
}