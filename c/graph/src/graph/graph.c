#include "graph/graph.h"
#include "graph/iter.h"
#include "private/graph_detail.h"
#include <stdlib.h>
#include <string.h>

GraphSize graphGetGraphSize() { return sizeof(Graph); }

static void graphManagerInit(GraphManager *mng, const GraphBool directed,
                             const GraphSize capacity,
                             const GraphSize headNum) {
  mng->range = 0;
  mng->freeHead = 0;
  mng->iterHead = malloc(headNum * sizeof(GraphId));
  memset(mng->iterHead, INVALID_ID, headNum * sizeof(GraphId));
  if (directed) {
    mng->next = mng->buff = malloc(capacity * sizeof(GraphId));
  } else {
    mng->buff = malloc(2 * capacity * sizeof(GraphId));
    mng->next = mng->buff + capacity;
  }
  mng->next[capacity - 1] = INVALID_ID;
  for (GraphId i = capacity - 1; i; --i) mng->next[i - 1] = i;
}

void graphInit(Graph *const graph, const GraphBool directed,
               const GraphSize vertCap, const GraphSize edgeCap) {
  graph->vertCap = vertCap;
  graph->edgeCap = edgeCap;
  graph->edgeNum = graph->vertNum = 0;
  graph->endpts = malloc(edgeCap * sizeof(GraphEdgeEndpoint));
  graphManagerInit(&graph->vertMng, 1, vertCap, 1);
  graphManagerInit(&graph->edgeMng, directed, edgeCap, vertCap);
  graph->vertAttr = graph->edgeAttr = NULL;
}

static inline void destroyManager(const GraphManager *mng) {
  free(mng->iterHead);
  free(mng->buff);
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
  destroyManager(&graph->vertMng);
  destroyManager(&graph->edgeMng);
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
