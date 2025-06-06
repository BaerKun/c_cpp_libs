#include "flow_network/max_flow.h"
#include "queue.h"
#include <stdlib.h>
#include <string.h>

typedef struct ResidualEdge_ ResidualEdge, *ResidualEdgePtr;
struct ResidualEdge_ {
  VertexId source, target;
  FlowType capacity, flow;
  NetworkEdgePtr parallel;
  ResidualEdgePtr *prevNext;
  ResidualEdgePtr next;
};

static void unlink(const ResidualEdgePtr edge) {
  const ResidualEdgePtr next = edge->next;
  *edge->prevNext = next;
  if (next != NULL) next->prevNext = edge->prevNext;
}

static void insert(ResidualEdgePtr *const prevNext, const ResidualEdgePtr edge) {
  const ResidualEdgePtr next = *prevNext;
  edge->next = next;
  *prevNext = edge;
  edge->prevNext = prevNext;
  if (next != NULL) next->prevNext = &edge->next;
}

static ResidualEdgePtr *createResidualNetwork(const Network *network) {
  ResidualEdgePtr *residual = malloc(sizeof(ResidualEdgePtr) * network->vertexNum +
                                     sizeof(ResidualEdge) * network->edgeNum);
  ResidualEdgePtr edges = (ResidualEdgePtr)(residual + network->vertexNum);
  for (int i = 0; i < network->vertexNum; i++) {
    residual[i] = NULL;
    for (NetworkEdgePtr edge = network->vertices[i].outEdges; edge != NULL; edge = edge->next) {
      const ResidualEdgePtr copy = edges++;
      copy->source = i;
      copy->target = edge->target;
      copy->capacity = edge->capacity;
      copy->flow = 0;
      copy->parallel = edge;
      insert(residual + i, copy);
    }
  }
  return residual;
}

static int bfs(const ResidualEdgePtr *residual, const QueuePtr queue, const VertexId source,
               const VertexId sink, ResidualEdgePtr *const parent) {
  queueClear(queue);
  enqueue(queue, source);
  while (!queueEmpty(queue)) {
    const VertexId vertex = *queueFront(queue);
    dequeue(queue);

    for (ResidualEdgePtr edge = residual[vertex]; edge != NULL; edge = edge->next) {
      const VertexId target = edge->target;
      if (parent[target] != NULL) continue;

      parent[target] = edge;
      if (target == sink) return 1;

      enqueue(queue, target);
    }
  }
  return 0;
}

static FlowType pathFlow(const ResidualEdgePtr *parent, const VertexId sink) {
  FlowType minFlow = UNREACHABLE;
  for (ResidualEdgePtr edge = parent[sink]; edge != NULL; edge = parent[edge->source]) {
    if (edge->capacity - edge->flow < minFlow) minFlow = edge->capacity - edge->flow;
  }
  return minFlow;
}

FlowType EdmondKarpMaxFlow(const Network *network, const VertexId source, const VertexId sink) {
  Queue queue;
  ResidualEdgePtr *residual = createResidualNetwork(network);
  ResidualEdgePtr *parent = malloc(sizeof(NetworkEdgePtr) * network->vertexNum);
  queueInit(&queue, network->vertexNum);

  FlowType maxFlow = 0;
  while (1) {
    memset(parent, 0, sizeof(NetworkEdgePtr) * network->vertexNum);
    if (!bfs(residual, &queue, source, sink, parent)) break;

    const FlowType flow = pathFlow(parent, sink);
    for (ResidualEdgePtr edge = parent[sink], prev; edge != NULL; edge = prev) {
      prev = parent[edge->source];
      edge->flow += flow;
      if (edge->target == edge->parallel->target) {
        edge->parallel->flow += flow;
      } else {
        edge->parallel->flow -= flow;
      }

      if (edge->flow == edge->capacity) {
        // 反转
        const VertexId tmp = edge->target;
        edge->target = edge->source;
        edge->source = tmp;
        edge->flow = 0;
        unlink(edge);
        insert(residual + edge->source, edge);
      }
    }
    maxFlow += flow;
  }

  free(residual);
  free(parent);
  queueFreeData(&queue);
  return maxFlow;
}