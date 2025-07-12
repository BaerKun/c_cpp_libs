#include "graph/iter.h"
#include "private/attribute.h"
#include "private/manager.h"
#include <stdlib.h>

GraphSize graphGetGraphSize() { return sizeof(Graph); }

void graphInit(Graph *const graph, const GraphBool directed,
               const GraphSize vertCap, const GraphSize edgeCap) {
  graph->vertCap = vertCap;
  graph->edgeCap = edgeCap;
  graph->edgeNum = graph->vertNum = 0;
  graphManagerInit(&graph->manager, directed, vertCap, edgeCap);
  graph->vertAttr = graph->edgeAttr = NULL;
}

void graphDestroy(const Graph *const graph) {
  graphManagerDestroy(&graph->manager);
  graphAttrDestroyList(graph->vertAttr);
  graphAttrDestroyList(graph->edgeAttr);
}

GraphId graphAddVert(Graph *const graph) {
  if (graph->vertNum++ == graph->vertCap) {
    // realloc
  }
  return graphManagerNewVert(&graph->manager);
}

void graphReserveVert(Graph *graph, const GraphSize num){
  graph->vertNum += num;
  if (graph->vertNum > graph->vertCap) {
    // realloc
  }
  for (GraphSize i = 0; i != num; ++i) {
    graphManagerNewVert(&graph->manager);
  }
}

GraphId graphAddEdge(Graph *const graph, const GraphId from, const GraphId to,
                     const GraphBool directed) {
  if (graph->edgeNum++ == graph->edgeCap) {
    // realloc
  }
  return graphManagerNewEdge(&graph->manager, from, to, directed);
}

void graphDeleteVert(Graph *graph, const GraphId id) {
  graphManagerDeleteVert(&graph->manager, id);
  --graph->vertNum;
}

void graphDeleteEdge(Graph *graph, const GraphId id) {
  graphManagerDeleteEdge(&graph->manager, id);
  --graph->edgeNum;
}

void graphGetIdRange(const Graph *graph, GraphSize *vert, GraphSize *edge) {
  if (vert) *vert = graph->manager.view.vertRange;
  if (edge) *edge = graph->manager.view.edgeRange;
}

void graphGetIndegree(const Graph *const graph, GraphInt indegree[]) {
  const GraphView *view = VIEW(graph);
  memset(indegree, 0, view->vertRange * sizeof(GraphInt));

  GraphIter *iter = graphIterFromView(view);
  GraphId from, id, to;
  while (graphIterNextVert(iter, &from)) {
    while (graphIterNextEdge(iter, from, &id, &to)) {
      ++indegree[to];
    }
  }
  graphIterRelease(iter);
}
