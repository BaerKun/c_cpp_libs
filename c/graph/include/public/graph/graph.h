#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include "graph/type.h"

void graphInit(Graph *graph, GraphSize vertCap, GraphSize edgeCap);
void graphDestroy(const Graph *graph);

GraphId graphAddVertex(Graph *graph);
GraphId graphAddEdge(Graph *graph, GraphId from, GraphId to,
                     GraphBool directed);

void *graphGetEdgeAttribute(const Graph *graph, const char name[16]);
void graphAddEdgeAttribute(Graph *graph, const char name[16],
                           GraphSize sizeOfElem);

void graphGetIdRange(const Graph *graph, GraphSize *vert, GraphSize *edge);
void graphGetIndegree(const Graph *graph, GraphInt indegree[]);

void gvIterInit(const Graph *graph, GraphIter *iter);
void geIterInit(const Graph *graph, GraphIter *iter, GraphId from);

static inline GraphId gvIterCurr(const GraphIter *const iter) {
  return iter->curr;
}

static inline GraphEdge geIterCurr(const Graph *const graph,
                                   const GraphIter *const iter) {
  if (iter->curr < graph->edgeCap)
    return (GraphEdge){iter->curr, graph->endpts[iter->curr].to};
  const GraphId id = iter->curr - graph->edgeCap;
  return (GraphEdge){id, graph->endpts[id].from};
}

static inline GraphBool ghIterEnd(const GraphIter *const iter) {
  return iter->curr == -1;
}

static inline void ghIterNext(GraphIter *const iter) {
  iter->curr = iter->next[iter->curr];
}

#endif // GRAPH_GRAPH_H