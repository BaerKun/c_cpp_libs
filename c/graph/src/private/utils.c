#include "graph/iter.h"
#include "private/graph_detail.h"
#include "private/queue.h"

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
