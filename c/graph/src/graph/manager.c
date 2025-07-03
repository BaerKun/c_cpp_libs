#include "graph/type.h"
#include "private/id_manager.h"

GraphId graphAddVertex(Graph *const graph) {
  if (graph->vertNum++ == graph->vertCap) {
    // realloc
  }
  VertManager *mng = &graph->manager->vert;
  const GraphId id = mng->freeHead;
  mng->freeHead = mng->next[id];
  mng->next[id] = mng->iterHead;
  mng->iterHead = id;
  if (mng->freeHead > mng->max) mng->max = mng->freeHead;
  return id;
}

GraphId graphAddEdge(Graph *const graph, const GraphId from, const GraphId to,
                     const GraphBool directed) {
  if (graph->edgeNum++ == graph->edgeCap) {
    // realloc
  }

  EdgeManager *const mng = &graph->manager->edge;
  const GraphId id = mng->freeHead;
  mng->freeHead = mng->next[id];
  mng->next[id] = mng->iterHead[from];
  mng->iterHead[from] = id;
  if (mng->freeHead > mng->max) mng->max = mng->freeHead;

  if (!directed) {
    const GraphId reverse = id + graph->edgeCap;
    mng->next[reverse] = mng->iterHead[to];
    mng->iterHead[to] = reverse;
  }
  return id;
}