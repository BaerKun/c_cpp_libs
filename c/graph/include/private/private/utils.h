#ifndef UTILS_H
#define UTILS_H

#include "graph_detail.h"
#include "queue.h"

void graphIndegreeInit(GraphIter *iter, const GraphInt indegree[],
                       GraphQueue *queue);

GraphId *graphFind(GraphId *next, GraphId *head, GraphId id);

static inline void graphUnlink(const GraphId *next, GraphId *predNext) {
  *predNext = next[*predNext];
}

static inline void graphInsert(GraphId *next, GraphId *predNext,
                               const GraphId id) {
  next[id] = *predNext;
  *predNext = id;
}

#endif // UTILS_H
