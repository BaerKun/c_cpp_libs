#include "private/graph_detail.h"
#include "private/queue.h"
#include "graph/iter.h"
#include <stdlib.h>
#include <string.h>

void graphIndegreeInit(GraphIter *iter, const GraphInt indegree[],
                       GraphQueue *queue);

void topoPath(const Graph *const graph, const GraphInt indegree[],
              GraphId predecessor[]) {
  const GraphView *view = VIEW(graph);
  GraphIter *iter = graphIterFromView(view);
  GraphQueue *queue = graphNewQueue(graph->vertNum);
  graphIndegreeInit(iter, indegree, queue);
  GraphInt *copyIndeg = malloc(view->vertRange * sizeof(GraphInt));
  memcpy(copyIndeg, indegree, view->vertRange * sizeof(GraphInt));
  memset(predecessor, INVALID_ID, view->vertRange * sizeof(GraphId));

  GraphInt counter = 0;
  GraphId id, to;
  while (!graphQueueEmpty(queue)) {
    const GraphId from = graphQueuePop(queue);
    ++counter;
    while (graphIterNextEdge(iter, from, &id, &to)) {
      if (predecessor[to] == -1) predecessor[to] = from;
      if (--copyIndeg[to] == 0) graphQueuePush(queue, to);
    }
  }

  if (counter != graph->vertNum) {/* ERROR: 圈 */}

  free(copyIndeg);
  graphIterRelease(iter);
  graphQueueRelease(queue);
}

void topoSort(const Graph *const graph, const GraphInt indegree[],
              GraphId sort[]) {
  const GraphView *view = VIEW(graph);
  GraphIter *iter = graphIterFromView(view);
  GraphQueue *queue = graphNewQueue(graph->vertNum);
  graphIndegreeInit(iter, indegree, queue);
  GraphInt *copyIndeg = malloc(view->vertRange * sizeof(GraphInt));
  memcpy(copyIndeg, indegree, view->vertRange * sizeof(GraphInt));

  GraphInt counter = 0;
  GraphId id, to;
  while (!graphQueueEmpty(queue)) {
    const GraphId from = graphQueuePop(queue);
    sort[counter++] = from;

    while (graphIterNextEdge(iter, from, &id, &to)) {
      if (--copyIndeg[to] == 0) graphQueuePush(queue, to);
    }
  }

  if (counter != graph->vertNum) {/* ERROR: 圈 */}

  free(copyIndeg);
  graphIterRelease(iter);
  graphQueueRelease(queue);
}
