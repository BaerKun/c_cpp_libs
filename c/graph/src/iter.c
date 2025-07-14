#include "graph/iter.h"
#include "private/view.h"
#include <stdlib.h>
#include <string.h>

GraphIter *graphGetIter(const Graph *graph) {
  return graphIterFromView(VIEW(graph));
}

void graphIterRelease(GraphIter *iter) { free(iter); }

void graphIterResetVert(GraphIter *iter) {
  iter->vertCurr = iter->view->vertHead;
}

void graphIterResetEdge(GraphIter *iter, const GraphId from) {
  if (from == INVALID_ID) {
    memcpy(iter->edgeCurr, iter->view->edgeHead,
           iter->view->vertRange * sizeof(GraphId));
  } else {
    iter->edgeCurr[from] = iter->view->edgeHead[from];
  }
}

void graphIterCurr(const GraphIter *iter, GraphId *from, GraphId *eid,
                   GraphId *to) {
  *from = iter->vertCurr;
  if (*from == INVALID_ID) return;
  *eid = iter->edgeCurr[*from];
  if (*eid == INVALID_ID) return;
  forward(iter->view, *eid, eid, to);
}

GraphBool graphIterNextVert(GraphIter *iter, GraphId *vid) {
  if (iter->vertCurr == INVALID_ID) return GRAPH_FALSE;
  *vid = iter->vertCurr;
  iter->vertCurr = iter->view->vertNext[iter->vertCurr];
  return GRAPH_TRUE;
}

GraphBool graphIterNextEdge(GraphIter *iter, const GraphId from, GraphId *eid,
                            GraphId *to) {
  GraphId *curr = iter->edgeCurr + from;
  if (*curr == INVALID_ID) return GRAPH_FALSE;
  forward(iter->view, *curr, eid, to);
  *curr = iter->view->edgeNext[*curr];
  return GRAPH_TRUE;
}