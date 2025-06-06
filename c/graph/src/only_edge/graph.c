#include "only_edge/graph.h"
#include <stdio.h>
#include <stdlib.h>

OnlyEdgeGraphPtr newGraph(const int edgeCapacity, const int vertexNum) {
  const OnlyEdgeGraphPtr graph = malloc(sizeof(OnlyEdgeGraph));

  for (graph->edgeCapacity = 1; edgeCapacity > graph->edgeCapacity;
       graph->edgeCapacity *= 2);

  graph->edges = malloc(sizeof(OnlyEdge) * graph->edgeCapacity);
  graph->edgeNum = 0;
  graph->vertexNum = vertexNum;

  return graph;
}

void graphDestroy(const OnlyEdgeGraphPtr graph) {
  free(graph->edges);
  free(graph);
}

void graphAddEdge(const OnlyEdgeGraphPtr graph, const VertexId vertex1,
                  const VertexId vertex2, const EdgeData data) {
  if (graph->edgeNum == graph->edgeCapacity) {
    fputs("graphAddEdge:Graph is full\n", stderr);
    return;
  }

  graph->edges[graph->edgeNum++] = (OnlyEdge){vertex1, vertex2, data};
}
