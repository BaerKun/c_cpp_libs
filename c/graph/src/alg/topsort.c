#include "graph/iter.h"
#include "private/graph_detail.h"
#include "private/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static GraphInt *init(const Graph *graph, const GraphInt indegree[],
                      GraphIter *iter, GraphQueue *queue) {
  const GraphSize vertRange = graph->vertMng.range;
  indegreeInit(iter, indegree, queue);
  graphIterResetEdge(graph, iter, INVALID_ID);

  GraphInt *copy = malloc(vertRange * sizeof(GraphInt));
  memcpy(copy, indegree, vertRange * sizeof(GraphInt));
  return copy;
}

void topoPath(const Graph *const graph, const GraphInt indegree[],
              GraphId predecessor[]) {
  GraphIter *iter = graphGetIter(graph);
  GraphQueue *queue = graphQueueCreate(graph->vertNum);
  GraphInt *copyIndeg = init(graph, indegree, iter, queue);
  memset(predecessor, INVALID_ID, graph->vertMng.range * sizeof(GraphId));

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

  if (counter != graph->vertNum) {
    fputs("buildTopPath: Has Cycle\n", stderr);
  }

  free(copyIndeg);
  graphIterRelease(iter);
  graphQueueRelease(queue);
}

void topoSort(const Graph *const graph, const GraphInt indegree[],
              GraphId sort[]) {
  GraphIter *iter = graphGetIter(graph);
  GraphQueue *queue = graphQueueCreate(graph->vertNum);
  GraphInt *copyIndeg = init(graph, indegree, iter, queue);

  GraphInt counter = 0;
  GraphId id, to;
  while (!graphQueueEmpty(queue)) {
    const GraphId from = graphQueuePop(queue);
    sort[counter++] = from;

    while (graphIterNextEdge(iter, from, &id, &to)) {
      if (--copyIndeg[to] == 0) graphQueuePush(queue, to);
    }
  }

  if (counter != graph->vertNum) {
    fputs("topSort: Has Cycle\n", stderr);
  }

  free(copyIndeg);
  graphIterRelease(iter);
  graphQueueRelease(queue);
}
