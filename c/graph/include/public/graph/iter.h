#ifndef GRAPH_ITER_H
#define GRAPH_ITER_H

#include "type.h"

typedef struct {
  GraphId vertCurr;
  GraphId *edgeCurr; // id
  const GraphId *vertNext, *edgeNext;
  const GraphEdgeEndpoint *endpts;
} GraphIter;

void graphIterInit(const Graph *graph, GraphIter *iter);
void graphIterRelease(const GraphIter *iter);
void graphIterResetVert(const Graph *graph, GraphIter *iter);
// 重置迭代边，INVALID_ID -> 重置全部
void graphIterResetEdge(const Graph *graph, const GraphIter *iter,
                        GraphId from);

static inline GraphBool graphIterNextVert(GraphIter *iter, GraphId *id) {
  if (iter->vertCurr == INVALID_ID) return GRAPH_FALSE;
  *id = iter->vertCurr;
  iter->vertCurr = iter->vertNext[iter->vertCurr];
  return GRAPH_TRUE;
}

static inline GraphBool graphIterNextEdge(const GraphIter *iter,
                                          const GraphId from, GraphId *id,
                                          GraphId *to) {
  GraphId *curr = iter->edgeCurr + from;
  if (*curr == INVALID_ID) return GRAPH_FALSE;
  if (*curr >= 0) {
    *id = *curr;
    *to = iter->endpts[*id].to;
  } else {
    *id = ~*curr;
    *to = iter->endpts[*id].from;
  }
  *curr = iter->edgeNext[*curr];
  return GRAPH_TRUE;
}

#endif // GRAPH_ITER_H
