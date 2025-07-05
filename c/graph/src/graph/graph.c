#include "graph/graph.h"
#include "graph/iter.h"
#include "private/graph_detail.h"
#include <stdlib.h>
#include <string.h>

GraphSize graphGetGraphSize() {
  return sizeof(Graph);
}

static void vertManagerInit(GraphVertManager *mng, const GraphSize vertCap) {
  mng->iterHead = INVALID_ID;
  mng->idMng.range = 0;
  mng->idMng.freeHead = 0;
  mng->idMng.next = malloc(vertCap * sizeof(GraphId));
  mng->idMng.next[vertCap - 1] = INVALID_ID;
  for (GraphId i = vertCap - 1; i; --i) mng->idMng.next[i - 1] = i;
}

static void edgeManagerInit(GraphEdgeManager *mng, const GraphBool directed,
                            const GraphSize vertCap, const GraphSize edgeCap) {
  mng->directed = directed;
  mng->iterHead = malloc(vertCap * sizeof(GraphId));
  memset(mng->iterHead, INVALID_ID, vertCap * sizeof(GraphId));
  mng->idMng.range = 0;
  mng->idMng.freeHead = 0;
  if (directed) {
    mng->idMng.next = mng->buff = malloc(edgeCap * sizeof(GraphId));
  } else {
    mng->buff = malloc(2 * edgeCap * sizeof(GraphId));
    mng->idMng.next = mng->buff + edgeCap;
  }
  mng->idMng.next[edgeCap - 1] = INVALID_ID;
  for (GraphId i = edgeCap - 1; i; --i) mng->idMng.next[i - 1] = i;
}

void graphInit(Graph *const graph, const GraphBool directed,
               const GraphSize vertCap, const GraphSize edgeCap) {
  graph->vertCap = vertCap;
  graph->edgeCap = edgeCap;
  graph->edgeNum = graph->vertNum = 0;
  graph->endpts = malloc(edgeCap * sizeof(GraphEdgeEndpoint));
  vertManagerInit(&graph->vertMng, vertCap);
  edgeManagerInit(&graph->edgeMng, directed, vertCap, edgeCap);
  graph->vertAttr = graph->edgeAttr = NULL;
}

static void destroyAttributeList(Attribute *attr) {
  for (Attribute *next; attr; attr = next) {
    next = attr->next;
    free(attr->vector);
    free(attr);
  }
}

void graphDestroy(const Graph *const graph) {
  free(graph->endpts);
  free(graph->vertMng.idMng.next);
  free(graph->edgeMng.iterHead);
  free(graph->edgeMng.buff);
  destroyAttributeList(graph->vertAttr);
  destroyAttributeList(graph->edgeAttr);
}

void graphGetIndegree(const Graph *const graph, GraphInt indegree[]) {
  memset(indegree, 0, graph->vertCap * sizeof(GraphInt));

  GraphIter iter;
  graphIterInit(graph, &iter);
  GraphId from, to, edge;
  while (graphIterNextVert(&iter, &from)) {
    while (graphIterNextEdge(&iter, from, &edge, &to)) {
      ++indegree[to];
    }
  }
  graphIterRelease(&iter);
}
