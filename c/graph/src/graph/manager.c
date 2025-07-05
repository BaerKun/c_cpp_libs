#include "graph/type.h"
#include "private/graph_detail.h"

void graphGetIdRange(const Graph *graph, GraphSize *vert, GraphSize *edge) {
  if (vert) *vert = graph->vertMng.idMng.range;
  if (edge) *edge = graph->edgeMng.idMng.range;
}

static GraphId mngGetNew(IdManager *mng, GraphId *head) {
  const GraphId id = mng->freeHead;
  mng->freeHead = mng->next[id];
  mng->next[id] = *head;
  *head = id;
  if (mng->freeHead > mng->range) mng->range = mng->freeHead;
  return id;
}

GraphId graphAddVert(Graph *const graph) {
  if (graph->vertNum++ == graph->vertCap) {
    // realloc
  }
  return mngGetNew(&graph->vertMng.idMng, &graph->vertMng.iterHead);
}

GraphId graphAddEdge(Graph *const graph, const GraphId from, const GraphId to,
                     const GraphBool directed) {
  if (graph->edgeNum++ == graph->edgeCap) {
    // realloc
  }

  GraphEdgeManager *const mng = &graph->edgeMng;
  const GraphId id = mngGetNew(&mng->idMng, mng->iterHead + from);
  graph->endpts[id] = (GraphEdgeEndpoint){from, to};
  if (!directed) {
    const GraphId reverse = ~id;
    mng->idMng.next[reverse] = mng->iterHead[to];
    mng->iterHead[to] = reverse;
  }
  return id;
}

static GraphId *mngFind(const IdManager *const mng, GraphId *head,
                        const GraphId id) {
  GraphId *predNext = head;
  while (*predNext != INVALID_ID && *predNext != id)
    predNext = mng->next + *predNext;
  return *predNext == INVALID_ID ? 0 : predNext;
}

static void mngDelete(IdManager *const mng, GraphId *head, const GraphId id) {
  GraphId *predNext = mngFind(mng, head, id);
  if (predNext) {
    *predNext = mng->next[id];
    mng->next[id] = mng->freeHead;
    mng->freeHead = id;
  }
}

void graphDeleteVert(Graph *graph, const GraphId id) {
  mngDelete(&graph->vertMng.idMng, &graph->vertMng.iterHead, id);
  --graph->vertNum;
}

void graphDeleteEdge(Graph *graph, const GraphId id) {
  GraphEdgeManager *const mng = &graph->edgeMng;
  const GraphEdgeEndpoint endpoint = graph->endpts[id];
  mngDelete(&mng->idMng, mng->iterHead + endpoint.from, id);

  const GraphId reverse = ~id;
  if (!mng->directed) {
    GraphId *predNext =
        mngFind(&mng->idMng, mng->iterHead + endpoint.to, reverse);
    if (predNext) *predNext = mng->idMng.next[reverse];
  }
  --graph->edgeNum;
}
