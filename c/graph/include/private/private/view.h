#ifndef GRAPH_VIEW_H
#define GRAPH_VIEW_H

#include "graph_detail.h"

#define REVERSE(did) ((did) ^ 1)

GraphId *graphViewCopy(const GraphView *view, GraphView *copy, GraphBool vert,
                       GraphBool edge);

GraphId *graphFind(GraphId *next, GraphId *head, GraphId id);

static inline void graphUnlink(const GraphId *next, GraphId *predNext) {
  *predNext = next[*predNext];
}

static inline void graphInsert(GraphId *next, GraphId *predNext,
                               const GraphId id) {
  next[id] = *predNext;
  *predNext = id;
}

static inline void graphInsertEdge(const GraphView *view, const GraphId from,
                                   const GraphId did) {
  graphInsert(view->edgeNext, view->edgeHead + from, did);
}

static inline GraphBool graphIterNextDirect(GraphIter *iter, const GraphId from,
                                            GraphId *did) {
  GraphId *curr = iter->edgeCurr + from;
  if (*curr == INVALID_ID) return GRAPH_FALSE;
  *did = *curr;
  *curr = iter->view->edgeNext[*curr];
  return GRAPH_TRUE;
}

static inline void parse(const GraphView *view, const GraphId did, GraphId *eid,
                         GraphId *to) {
  if (view->directed) {
    *eid = did;
    *to = view->endpts[did].to;
  } else {
    *eid = did >> 1;
    *to = ((GraphId *)view->endpts)[did];
  }
}

#endif // GRAPH_VIEW_H
