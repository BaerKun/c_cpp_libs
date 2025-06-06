#include "flow_network/network.h"
#include <stdlib.h>

void networkInit(const NetworkPtr network, const int vertexNum) {
  network->vertices = malloc(vertexNum * sizeof(NetworkVertex));
  network->vertexNum = vertexNum;
  network->edgeNum = 0;
}

void networkDestroy(const NetworkPtr network) {
  const NetworkVertexPtr end = network->vertices + network->vertexNum;
  for (NetworkVertexPtr vertex = network->vertices; vertex != end; vertex++) {
    for (NetworkEdgePtr edge = vertex->outEdges, next; edge; edge = next) {
      next = edge->next;
      free(edge);
    }
  }
  free(network->vertices);
}

void networkAddEdge(const NetworkPtr network, const VertexId source, const VertexId target,
                    const FlowType capacity, const EdgeData data) {
  const NetworkEdgePtr edge = malloc(sizeof(NetworkEdge));
  edge->target = target;
  edge->capacity = capacity;
  edge->flow = 0;
  edge->data = data;

  edge->next = network->vertices[source].outEdges;
  network->vertices[source].outEdges = edge;

  ++network->edgeNum;
}