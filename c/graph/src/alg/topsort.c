#include "graph/graph.h"
#include "private/indegree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void topoPath(const Graph *const graph, const GraphInt indegree[],
              GraphId predecessor[]) {
  GraphInt counter = 0;
  GraphQueue queue;
  GraphInt *copyIndeg = indegreeInit(indegree, &queue, graph->vertCap);
  memset(predecessor, 255, graph->vertCap * sizeof(GraphId)); // -1

  while (!graphQueueEmpty(&queue)) {
    const GraphId from = graphQueuePop(&queue);
    ++counter;

    for (GraphEdgePtr edge = graph->adjList[from]; edge; edge = edge->next) {
      const GraphId to = edge->to;
      if (predecessor[to] == -1) predecessor[to] = from;
      if (--copyIndeg[to] == 0) graphQueuePush(&queue, to);
    }
  }

  if (counter != graph->vertNum) {
    fputs("buildTopPath: Has Cycle\n", stderr);
  }

  free(copyIndeg);
  graphQueueRelease(&queue);
}

void topoSort(const Graph *const graph, const GraphInt indegree[],
              GraphId sort[]) {
  GraphQueue queue;
  GraphInt *copyIndeg = indegreeInit(indegree, &queue, graph->vertCap);

  GraphInt counter = 0;
  while (!graphQueueEmpty(&queue)) {
    const GraphId from = graphQueuePop(&queue);
    sort[counter++] = from;

    for (GraphEdgePtr edge = graph->adjList[from]; edge; edge = edge->next) {
      if (--copyIndeg[edge->to] == 0) graphQueuePush(&queue, edge->to);
    }
  }

  if (counter != graph->vertNum) {
    fputs("topSort: Has Cycle\n", stderr);
  }

  free(copyIndeg);
  graphQueueRelease(&queue);
}
