#include "graph/adj_list/topsort.h"
#include "graph/adj_list/indegree.h"
#include <stdio.h>
#include <string.h>

void buildTopPath(const Graph *const graph, const GraphInt indegree[],
                  GraphId predecessor[]) {
  GraphInt counter = 0;
  Queue queue;
  GraphInt *copyIndeg;
  indegreeInit(indegree, graph->vertCap, &copyIndeg, &queue);
  memset(predecessor, 255, graph->vertCap * sizeof(GraphId)); // -1

  while (queueEmpty(&queue)) {
    const GraphId from = *queueFront(&queue);
    dequeue(&queue);
    ++counter;

    for (GraphEdgePtr edge = graph->adjList[from]; edge; edge = edge->next) {
      const GraphId to = edge->to;
      if (predecessor[to] == -1) {
        predecessor[to] = from;
      }
      if (--copyIndeg[to] == 0) {
        enqueue(&queue, to);
      }
    }
  }

  if (counter != graph->vertNum) {
    fputs("buildTopPath: Has Cycle\n", stderr);
  }

  free(copyIndeg);
  queueFreeData(&queue);
}

void topoSort(const Graph *const graph, const GraphInt indegree[],
              GraphId sort[]) {
  Queue queue;
  GraphInt *copyIndeg;
  indegreeInit(indegree, graph->vertCap, &copyIndeg, &queue);

  GraphInt counter = 0;
  while (queueEmpty(&queue)) {
    const GraphId from = *queueFront(&queue);
    dequeue(&queue);
    sort[counter++] = from;

    for (GraphEdgePtr edge = graph->adjList[from]; edge; edge = edge->next) {
      if (--copyIndeg[edge->to] == 0) {
        enqueue(&queue, edge->to);
      }
    }
  }

  if (counter != graph->vertNum) {
    fputs("topSort: Has Cycle\n", stderr);
  }

  free(copyIndeg);
  queueFreeData(&queue);
}
