#include "private/indegree.h"
#include "graph/graph.h"
#include <stdlib.h>
#include <string.h>

GraphInt *indegreeInit(const Graph *graph, const GraphInt indegree[],
                       GraphQueue *const queue) {
  GraphSize vertRange;
  graphGetIdRange(graph, &vertRange, NULL);
  graphQueueInit(queue, graph->vertNum);
  GraphInt *const copy = malloc(vertRange * sizeof(GraphInt));
  memcpy(copy, indegree, vertRange * sizeof(GraphInt));

  GraphIter iter;
  gvIterInit(graph, &iter);
  while (!ghIterEnd(&iter)) {
    const GraphId v = gvIterCurr(&iter);
    if (indegree[v] == 0) graphQueuePush(queue, v);
    ghIterNext(&iter);
  }
  return copy;
}
