#ifndef GRAPH_VIEW_H
#define GRAPH_VIEW_H

#include "graph_detail.h"

#define REVERSE(edge) ((edge) ^ 1)

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

#endif // GRAPH_VIEW_H
