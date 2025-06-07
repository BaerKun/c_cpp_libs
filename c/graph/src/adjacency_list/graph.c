#include "adjacency_list/graph.h"
#include "adjacency_list/edge.h"
#include <stdlib.h>
#include <string.h>

void listGraphInit(const ListGraphPtr graph, const int type, const int vertCap) {
  graph->type = type;
  graph->edgeNum = 0;
  graph->vertCap = vertCap;
  graph->vertNum = 0;
  graph->vertices = malloc(vertCap * sizeof(ListVertex));
}

void listGraphDestroy(const ListGraphPtr graph) {
  const ListVertexPtr end = graph->vertices + graph->vertNum;
  for (ListVertexPtr vertex = graph->vertices; vertex != end; vertex++) {
    for (ListEdgePtr edge = vertex->outEdges, next; edge; edge = next) {
      next = edge->next;
      free(edge);
    }
  }
  free(graph->vertices);
}

void listGraphAddVertex(const ListGraphPtr graph, const VertexData data) {
  graph->vertices[graph->vertNum++] =
      (ListVertex){.indegree = 0, .outEdges = NULL, .userdata = data};
}

void listGraphReserve(const ListGraphPtr graph, const int vertNum) {
  while (graph->vertNum < vertNum) {
    graph->vertices[graph->vertNum++] =
        (ListVertex){.indegree = 0, .outEdges = NULL, .userdata = NULL};
  }
}

static ListEdgePtr newEdge(const int type, const VertexId target, const EdgeData data,
                           const WeightType value) {
  static const size_t edgeTailSize[] = {0, sizeof(WeightType), sizeof(FlowTail), sizeof(AnaTail)};

  const size_t tailSize = edgeTailSize[type];
  const ListEdgePtr edge = malloc(sizeof(ListEdge) + tailSize);
  edge->flag = 0;
  edge->target = target;
  edge->next = NULL;
  edge->userdata = data;
  if (tailSize > sizeof(WeightType)) {
    memset(edge->tail, 0, tailSize - sizeof(WeightType));
  }
  edge->tail->weight = value;
  return edge;
}

void listGraphAddEdge(const ListGraphPtr graph, const VertexId source, const VertexId target,
                      const EdgeData data, const WeightType value, const int directed) {
  const ListVertexPtr sourceVertex = graph->vertices + source;
  const ListVertexPtr targetVertex = graph->vertices + target;

  const ListEdgePtr edge = newEdge(graph->type, target, data, value);
  if (directed) {
    edge->reverse = NULL;
  } else {
    const ListEdgePtr reverse = newEdge(graph->type, source, data, value);
    reverse->reverse = edge;
    edge->reverse = reverse;

    edgeInsert(&targetVertex->outEdges, reverse);
    ++sourceVertex->indegree;
    ++graph->edgeNum;
  }
  edgeInsert(&sourceVertex->outEdges, edge);
  ++targetVertex->indegree;
  ++graph->edgeNum;
}
