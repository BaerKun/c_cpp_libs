#include "private/manager.h"
#include "private/view.h"
#include <stdlib.h>
#include <string.h>

void graphManagerInit(GraphManager *mgr, const GraphBool directed,
                      const GraphSize vertCap, const GraphSize edgeCap) {
  mgr->vertFree = mgr->edgeFree = 0;

  GraphView *const view = &mgr->view;
  view->vertRange = 0;
  view->vertHead = INVALID_ID;
  view->vertNext = malloc(vertCap * sizeof(GraphId));
  view->vertNext[vertCap - 1] = INVALID_ID;
  for (GraphId i = (GraphId)vertCap - 1; i; --i) view->vertNext[i - 1] = i;

  view->directed = directed;
  view->edgeRange = 0;
  view->edgeHead = malloc(vertCap * sizeof(GraphId));
  memset(view->edgeHead, INVALID_ID, vertCap * sizeof(GraphId));
  view->endpts = malloc(edgeCap * sizeof(GraphEndpoint));

  if (directed) {
    view->edgeNext = malloc(edgeCap * sizeof(GraphId));
    view->edgeNext[edgeCap - 1] = INVALID_ID;
    for (GraphId i = (GraphId)edgeCap - 1; i; --i) view->edgeNext[i - 1] = i;
  } else {
    view->edgeNext = malloc(2 * edgeCap * sizeof(GraphId));
    view->edgeNext[2 * (edgeCap - 1)] = INVALID_ID;
    for (GraphId i = 2 * ((GraphId)edgeCap - 1); i; i -= 2)
      view->edgeNext[i - 2] = i;
  }
}

void graphManagerDestroy(const GraphManager *mgr) {
  free(mgr->view.vertNext);
  free(mgr->view.edgeHead);
  free(mgr->view.edgeNext);
  free(mgr->view.endpts);
}

GraphId graphManagerNewVert(GraphManager *mgr) {
  GraphView *view = &mgr->view;
  const GraphId vid = mgr->vertFree;
  graphUnlink(view->vertNext, &mgr->vertFree);
  graphInsert(view->vertNext, &view->vertHead, vid);
  if (vid == view->vertRange) ++view->vertRange;
  return vid;
}

GraphId graphManagerNewEdge(GraphManager *mgr, const GraphId from,
                            const GraphId to, const GraphBool directed) {
  GraphView *view = &mgr->view;
  const GraphId did = mgr->edgeFree;
  graphUnlink(view->edgeNext, &mgr->edgeFree);
  graphInsertEdge(view, from, did);
  if (!directed) graphInsertEdge(view, to, REVERSE(did));

  const GraphId eid = view->directed ? did : (did >> 1);
  view->endpts[eid] = (GraphEndpoint){to, from};
  if (eid == view->edgeRange) ++view->edgeRange;
  return eid;
}

void graphManagerDeleteVert(GraphManager *mgr, const GraphId vid) {
  GraphView *view = &mgr->view;

  GraphId *predNext = graphFind(view->vertNext, &view->vertHead, vid);
  graphUnlink(view->vertNext, predNext);
  graphInsert(view->vertNext, &mgr->vertFree, vid);
  if (vid == view->vertRange - 1) view->vertRange = vid;
}

void graphManagerDeleteEdge(GraphManager *mgr, const GraphId eid) {
  GraphView *view = &mgr->view;
  const GraphId did = view->directed ? eid : (eid << 1);
  const GraphId to = view->endpts[eid].to;
  const GraphId from = view->endpts[eid].from;

  GraphId *predNext = graphFind(view->edgeNext, view->edgeHead + from, did);
  graphUnlink(view->edgeNext, predNext);
  graphInsert(view->edgeNext, &mgr->edgeFree, did);
  if (!view->directed) {
    predNext = graphFind(view->edgeNext, view->edgeHead + to, REVERSE(did));
    if (predNext) graphUnlink(view->edgeNext, predNext);
  }
  if (eid == view->edgeRange - 1) view->edgeRange = eid;
}