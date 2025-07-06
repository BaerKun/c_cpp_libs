#include "private/utils.h"

void indegreeInit(GraphIter *iter, const GraphInt indegree[],
                  GraphQueue *queue) {
  GraphId from, id, to;
  while (graphIterNextVert(iter, &from)) {
    while (graphIterNextEdge(iter, from, &id, &to)) {
      if (indegree[to] == 0) graphQueuePush(queue, to);
    }
  }
}
