#include "graph/iter.h"
#include "private/graph_detail.h"
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

static void helper(const GraphEndpoint *endpts, const GraphId directedId,
                   GraphId *id, GraphId *to) {
  if (directedId >= 0) {
    *id = directedId;
    *to = endpts[*id].to;
  } else {
    *id = ~directedId;
    *to = endpts[*id].from;
  }
}

void graphIterCurr(const GraphIter *iter, GraphId *from, GraphId *id,
                   GraphId *to) {
  *from = iter->vertCurr;
  if (*from == INVALID_ID) return;
  *id = iter->edgeCurr[*from];
  if (*id == INVALID_ID) return;
  helper(iter->view->endpts, *id, id, to);
}

GraphBool graphIterNextVert(GraphIter *iter, GraphId *id) {
  if (iter->vertCurr == INVALID_ID) return GRAPH_FALSE;
  *id = iter->vertCurr;
  iter->vertCurr = iter->view->vertNext[iter->vertCurr];
  return GRAPH_TRUE;
}

GraphBool graphIterNextEdge(GraphIter *iter, const GraphId from, GraphId *id,
                            GraphId *to) {
  GraphId *curr = iter->edgeCurr + from;
  if (*curr == INVALID_ID) return GRAPH_FALSE;
  helper(iter->view->endpts, *curr, id, to);
  *curr = iter->view->edgeNext[*curr];
  return GRAPH_TRUE;
}