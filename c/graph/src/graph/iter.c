#include "graph/iter.h"
#include "private/graph_detail.h"
#include <stdlib.h>
#include <string.h>

GraphIter *graphGetIter(const Graph *graph) {
  const GraphSize sizeOfEdgeHeads = graph->vertMng.range * sizeof(GraphId);
  GraphIter *iter = malloc(sizeof(GraphIter) + sizeOfEdgeHeads);
  iter->vertNext = graph->vertMng.next;
  iter->edgeNext = graph->edgeMng.next;
  iter->endpts = graph->endpts;
  iter->vertCurr = *graph->vertMng.iterHead;
  memcpy(iter->edgeCurr, graph->edgeMng.iterHead, sizeOfEdgeHeads);
  return iter;
}

void graphIterRelease(GraphIter *iter) { free(iter); }

void graphIterResetVert(const Graph *graph, GraphIter *iter) {
  iter->vertCurr = *graph->vertMng.iterHead;
}

void graphIterResetEdge(const Graph *graph, GraphIter *iter,
                        const GraphId from) {
  if (from == INVALID_ID) {
    memcpy(iter->edgeCurr, graph->edgeMng.iterHead,
           graph->vertMng.range * sizeof(GraphId));
  } else {
    iter->edgeCurr[from] = graph->edgeMng.iterHead[from];
  }
}

static void helper(const GraphEdgeEndpoint *endpts, const GraphId directedId,
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
  helper(iter->endpts, *id, id, to);
}

GraphBool graphIterNextVert(GraphIter *iter, GraphId *id) {
  if (iter->vertCurr == INVALID_ID) return GRAPH_FALSE;
  *id = iter->vertCurr;
  iter->vertCurr = iter->vertNext[iter->vertCurr];
  return GRAPH_TRUE;
}

GraphBool graphIterNextEdge(GraphIter *iter, const GraphId from, GraphId *id,
                            GraphId *to) {
  GraphId *curr = iter->edgeCurr + from;
  if (*curr == INVALID_ID) return GRAPH_FALSE;
  helper(iter->endpts, *curr, id, to);
  *curr = iter->edgeNext[*curr];
  return GRAPH_TRUE;
}