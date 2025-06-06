#include "adjacency_list/graph.h"
#include "adjacency_list/edge.h"
#include <stdlib.h>

void listGraphInit(const ListGraphPtr graph, const int capacity) {
  graph->vertices = malloc(capacity * sizeof(ListVertex));
  graph->capacity = capacity;
  graph->vertexNum = 0;
  graph->edgeNum = 0;
}

void listGraphDestroy(const ListGraphPtr graph) {
  const ListVertexPtr end = graph->vertices + graph->vertexNum;
  for (ListVertexPtr vertex = graph->vertices; vertex != end; vertex++) {
    for (ListEdgePtr edge = vertex->outEdges, next; edge; edge = next) {
      next = edge->next;
      free(edge);
    }
  }
  free(graph->vertices);
}

void listGraphAddEdge(const ListGraphPtr graph, const VertexId source, const VertexId target,
                  const WeightType weight, const EdgeData data, const int directed) {
  const ListVertexPtr sourceVertex = graph->vertices + source;
  const ListVertexPtr targetVertex = graph->vertices + target;

  const ListEdgePtr edge = malloc(sizeof(ListEdge));
  edge->flag = 0;
  edge->target = target;
  edge->weight = weight;
  edge->next = NULL;
  edge->data = data;

  if (directed) {
    edge->reverse = NULL;
  } else {
    const ListEdgePtr reverseEdge = malloc(sizeof(ListEdge));
    reverseEdge->flag = 0;
    reverseEdge->target = source;
    reverseEdge->weight = weight;
    reverseEdge->next = NULL;
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
