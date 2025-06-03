#include "adjacency_list/graph.h"
#include "adjacency_list/edge.h"
#include <stdlib.h>

void graphInit(const GraphPtr graph, const int capacity) {
  graph->vertices = malloc(capacity * sizeof(Vertex));
  graph->capacity = capacity;
  graph->vertexNum = 0;
  graph->edgeNum = 0;
}

void graphDestroy(const GraphPtr graph) {
  for (VertexPtr vertex = graph->vertices, end = vertex + graph->vertexNum;
       vertex != end; vertex++) {
    for (EdgePtr edge = vertex->outEdges, next; edge; edge = next) {
      next = edge->next;
      free(edge);
    }
  }
  free(graph->vertices);
}

void graphAddEdge(const GraphPtr graph, const VertexId source,
                  const VertexId target, const EdgeData data,
                  const int undirected) {
  const VertexPtr sourceVertex = graph->vertices + source;
  const VertexPtr targetVertex = graph->vertices + target;

  const EdgePtr edge = malloc(sizeof(Edge));
  edge->next = NULL;
  edge->target = target;
  edge->flag = 1;
  edge->data = data;

  if (!undirected) {
    edge->reverse = NULL;
  } else {
    const EdgePtr reverseEdge = malloc(sizeof(Edge));
    reverseEdge->next = NULL;
    reverseEdge->target = source;
    reverseEdge->flag = 1;
    reverseEdge->reverse = edge;
    reverseEdge->data = data;

    edge->reverse = reverseEdge;

    edgeInsert(&targetVertex->outEdges, reverseEdge);
    ++sourceVertex->indegree;
    ++graph->edgeNum;
  }
  edgeInsert(&sourceVertex->outEdges, edge);
  ++targetVertex->indegree;
  ++graph->edgeNum;
}
