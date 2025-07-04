#include "graph/type.h"
#include "private/id_manager.h"

void graphGetIdRange(const Graph *graph, GraphSize *vert, GraphSize *edge) {
  if (vert) *vert = graph->manager->vert.range;
  if (edge) *edge = graph->manager->edge.range;
}

static GraphId mngGetNew(IdManager *mng, const GraphId from) {
  const GraphId id = mng->freeHead;
  mng->freeHead = mng->next[id];
  mng->next[id] = mng->iterHead[from];
  mng->iterHead[from] = id;
  if (mng->freeHead > mng->range) mng->range = mng->freeHead;
  return id;
}

GraphId graphAddVertex(Graph *const graph) {
  if (graph->vertNum++ == graph->vertCap) {
    // realloc
  }
  return mngGetNew(&graph->manager->vert, 0);
}

GraphId graphAddEdge(Graph *const graph, const GraphId from, const GraphId to,
                     const GraphBool directed) {
  if (graph->edgeNum++ == graph->edgeCap) {
    // realloc
  }

  const GraphId id = mngGetNew(&graph->manager->edge, from);
  graph->endpts[id] = (GraphEdgeEndpoint){from, to};
  if (!directed) {
    const IdManager *const mng = &graph->manager->edge;
    const GraphId reverse = id + (GraphId)graph->edgeCap;
    mng->next[reverse] = mng->iterHead[to];
    mng->iterHead[to] = reverse;
  }
  return id;
}

static GraphId *mngFind(const IdManager *const mng, const GraphId from,
                        const GraphId id) {
  GraphId *predNext = mng->iterHead + from;
  while (*predNext != -1 && *predNext != id) {
    predNext = mng->next + *predNext;
  }
  return predNext;
}

static void mngDelete(IdManager *const mng, const GraphId from,
                      const GraphId id) {
  GraphId *predNext = mngFind(mng, from, id);
  if (*predNext != -1) {
    *predNext = mng->next[id];
    mng->next[id] = mng->freeHead;
    mng->freeHead = id;
  }
}

void graphDeleteVertex(Graph *graph, const GraphId id) {
  mngDelete(&graph->manager->vert, 0, id);
  --graph->vertNum;
}

void graphDeleteEdge(Graph *graph, const GraphId id) {
  IdManager *const mng = &graph->manager->edge;
  const GraphEdgeEndpoint endpoint = graph->endpts[id];
  mngDelete(mng, endpoint.from, id);

  const GraphId reverse = id + (GraphId)graph->edgeCap;
  if (mng->next[reverse] != -1) {
    GraphId *predNext = mngFind(mng, endpoint.to, reverse);
    *predNext = mng->next[reverse];
    mng->next[reverse] = -1;
  }
  --graph->edgeNum;
}

static inline void mngIterInit(const IdManager *const mng, GraphIter *iter,
                               const GraphId from) {
  iter->curr = mng->iterHead[from];
  iter->next = mng->next;
}

void gvIterInit(const Graph *const graph, GraphIter *const iter) {
  mngIterInit(&graph->manager->vert, iter, 0);
}

void geIterInit(const Graph *const graph, GraphIter *iter, const GraphId from) {
  mngIterInit(&graph->manager->edge, iter, from);
}