#include "private/graph_detail.h"
#include "private/queue.h"
#include "graph/iter.h"
#include <stdlib.h>
#include <string.h>

GraphIter *graphIterFromView(const GraphView *view) {
  GraphIter *iter =
      malloc(sizeof(GraphIter) + view->vertRange * sizeof(GraphId));
  iter->view = view;
  iter->vertCurr = view->vertHead;
  memcpy(iter->edgeCurr, view->edgeHead, view->edgeRange * sizeof(GraphId));
  return iter;
}

GraphId *graphFind(GraphId *next, GraphId *head, const GraphId id) {
  GraphId *predNext = head;
  while (*predNext != INVALID_ID && *predNext != id)
    predNext = next + *predNext;
  return *predNext == INVALID_ID ? 0 : predNext;
}

void graphIndegreeInit(GraphIter *iter, const GraphInt indegree[],
                  GraphQueue *queue) {
  GraphId from, id, to;
  while (graphIterNextVert(iter, &from)) {
    while (graphIterNextEdge(iter, from, &id, &to)) {
      if (indegree[to] == 0) graphQueuePush(queue, to);
    }
  }
  graphIterResetVert(iter);
  graphIterResetEdge(iter, INVALID_ID);
}
