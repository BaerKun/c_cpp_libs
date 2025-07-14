#include "graph/alg.h"
#include "graph/graph.h"
#include "graph/iter.h"
#include <stdio.h>
#include <stdlib.h>

int testIter() {
  Graph *graph = malloc(graphGetGraphSize());
  graphInit(graph, GRAPH_FALSE, 10, 10);

  for (GraphId i = 0; i < 5; i++) {
    graphAddVert(graph);
  }
  graphAddEdge(graph, 0, 1, GRAPH_FALSE);
  graphAddEdge(graph, 1, 2, GRAPH_TRUE);
  graphAddEdge(graph, 2, 3, GRAPH_TRUE);
  graphDeleteEdge(graph, 0);
  graphAddEdge(graph, 3, 4, GRAPH_FALSE);
  graphDeleteEdge(graph, 2);
  graphAddEdge(graph, 4, 0, GRAPH_TRUE);

  GraphIter *iter = graphGetIter(graph);
  GraphId from, to, edge;
  printf("from\teid\t\tto\n");
  while (graphIterNextVert(iter, &from)) {
    while (graphIterNextEdge(iter, from, &edge, &to)) {
      printf("%lld\t\t%lld\t\t%lld\n", from, to, edge);
    }
  }
  graphIterRelease(iter);
  graphDestroy(graph);
  return 1;
}

int testMaxFlow() {
  char buff[graphGetGraphSize()];
  Graph *graph = (Graph *)buff;
  graphInit(graph, GRAPH_TRUE, 10, 15);
  graphReserveVert(graph, 6);

  const FlowType capacity[9] = {3, 5, 1, 4, 2, 2, 1, 5, 2};
  graphAddEdge(graph, 0, 1, GRAPH_TRUE);
  graphAddEdge(graph, 0, 3, GRAPH_TRUE);
  graphAddEdge(graph, 1, 3, GRAPH_TRUE);
  graphAddEdge(graph, 1, 2, GRAPH_TRUE);
  graphAddEdge(graph, 4, 1, GRAPH_TRUE);
  graphAddEdge(graph, 3, 4, GRAPH_TRUE);
  graphAddEdge(graph, 4, 2, GRAPH_TRUE);
  graphAddEdge(graph, 2, 5, GRAPH_TRUE);
  graphAddEdge(graph, 4, 5, GRAPH_TRUE);

  FlowType flow[9];
  const FlowType maxFlow = EdmondsKarpMaxFlow(graph, capacity, flow, 0, 5);
  printf("max flow: %lld\n", maxFlow);
  for (GraphSize i = 0; i < 9; ++i) {
    printf("%lld ", flow[i]);
  }
  putchar('\n');
  graphDestroy(graph);
  return 1;
}

int main() {
  testIter();
  testMaxFlow();
}