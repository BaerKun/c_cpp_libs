#include "graph/type.h"
#include "private/graph_detail.h"

void graphGetIdRange(const Graph *graph, GraphSize *vert, GraphSize *edge) {
  if (vert) *vert = graph->vertMng.range;
  if (edge) *edge = graph->edgeMng.range;
}

static GraphId mngGetNew(GraphManager *mng, const GraphId from) {
  const GraphId id = mng->freeHead;
  mng->freeHead = mng->next[id];
  mng->next[id] = mng->iterHead[from];
  mng->iterHead[from] = id;
  if (mng->freeHead > mng->range) mng->range = mng->freeHead;
  return id;
}

GraphId graphAddVert(Graph *const graph) {
  if (graph->vertNum++ == graph->vertCap) {
    // realloc
  }
  return mngGetNew(&graph->vertMng, 0);
}

GraphId graphAddEdge(Graph *const graph, const GraphId from, const GraphId to,
                     const GraphBool directed) {
  if (graph->edgeNum++ == graph->edgeCap) {
    // realloc
  }

  GraphManager *const mng = &graph->edgeMng;
  const GraphId id = mngGetNew(mng, from);
  graph->endpts[id] = (GraphEdgeEndpoint){from, to};
  if (!directed) {
    const GraphId reverse = ~id;
    mng->next[reverse] = mng->iterHead[to];
    mng->iterHead[to] = reverse;
  }
  return id;
}

static GraphId *mngFind(const GraphManager *const mng, const GraphId head,
                        const GraphId id) {
  GraphId *predNext = mng->iterHead + head;
  while (*predNext != INVALID_ID && *predNext != id)
    predNext = mng->next + *predNext;
  return *predNext == INVALID_ID ? 0 : predNext;
}

static void mngDelete(GraphManager *const mng, const GraphId head,
                      const GraphId id) {
  GraphId *predNext = mngFind(mng, head, id);
  if (predNext) {
    *predNext = mng->next[id];
    mng->next[id] = mng->freeHead;
    mng->freeHead = id;
  }
}

void graphDeleteVert(Graph *graph, const GraphId id) {
  mngDelete(&graph->vertMng, 0, id);
  --graph->vertNum;
}

void graphDeleteEdge(Graph *graph, const GraphId id) {
  GraphManager *const mng = &graph->edgeMng;
  const GraphEdgeEndpoint endpoint = graph->endpts[id];
  mngDelete(mng, endpoint.from, id);

  const GraphId reverse = ~id;
  if (!graph->directed) {
    GraphId *predNext = mngFind(mng, endpoint.to, reverse);
    if (predNext) *predNext = mng->next[reverse];
  }
  --graph->edgeNum;
}
