#include "graph/graph.h"
#include "private/attribute.h"
#include "private/id_manager.h"
#include <stdlib.h>
#include <string.h>

static void idManagerInit(IdManager *mng, const GraphSize cap,
                          const GraphBool directed) {
  mng->range = mng->freeHead = 0;
  mng->next = malloc((directed ? cap : cap * 2) * sizeof(GraphId));
  for (GraphId i = 1; i < cap; ++i) mng->next[i - 1] = i;
  mng->next[cap - 1] = -1;
  if (!directed) memset(mng->next + cap, -1, cap * sizeof(GraphId));
}

static GraphManager *graphManagerCreate(const GraphSize vertCap,
                                        const GraphSize edgeCap) {
  GraphManager *const mng = malloc(sizeof(GraphManager));
  idManagerInit(&mng->vert, vertCap, 1);
  idManagerInit(&mng->edge, edgeCap, 0);
  mng->iterHead = malloc((edgeCap + 1) * sizeof(GraphId));
  memset(mng->iterHead, -1, (edgeCap + 1) * sizeof(GraphId));
  mng->vert.iterHead = mng->iterHead;
  mng->edge.iterHead = mng->iterHead + 1;
  return mng;
}

void graphInit(Graph *const graph, const GraphSize vertCap,
               const GraphSize edgeCap) {
  graph->vertCap = vertCap;
  graph->edgeCap = edgeCap;
  graph->edgeNum = graph->vertNum = 0;
  graph->endpts = malloc(edgeCap * sizeof(GraphEdgeEndpoint));
  graph->manager = graphManagerCreate(vertCap, edgeCap);
  graph->vertAttr = graph->edgeAttr = NULL;
}

static void destroyManager(GraphManager *const mng) {
  free(mng->iterHead);
  free(mng->vert.next);
  free(mng->edge.next);
  free(mng);
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
  destroyManager(graph->manager);
  destroyAttributeList(graph->vertAttr);
  destroyAttributeList(graph->edgeAttr);
}

void graphGetIndegree(const Graph *const graph, GraphInt indegree[]) {
  memset(indegree, 0, graph->vertCap * sizeof(GraphInt));

  GraphIter vertIter, edgeIter;
  gvIterInit(graph, &vertIter);
  while (!ghIterEnd(&vertIter)) {
    const GraphId from = gvIterCurr(&vertIter);
    geIterInit(graph, &edgeIter, from);
    while (!ghIterEnd(&edgeIter)) {
      const GraphEdge edge = geIterCurr(graph, &edgeIter);
      ++indegree[edge.to];
      ghIterNext(&edgeIter);
    }
    ghIterNext(&vertIter);
  }
}
