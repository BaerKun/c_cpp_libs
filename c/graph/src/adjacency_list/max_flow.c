#include "../../include/adjacency_list/max_flow.h"
#include "queue.h"
#include <stdlib.h>
#include <string.h>

typedef struct ResidualEdge_ ResidualEdge, *ResidualEdgePtr;
struct ResidualEdge_ {
  VertexId source, target;
  FlowType capacity, flow;
  ListEdgePtr parallel; // 对应的原网络边
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

// 残余网络
static ResidualEdgePtr *createResidualNetwork(const ListGraph *network) {
  ResidualEdgePtr *residual =
      malloc(sizeof(ResidualEdgePtr) * network->vertNum + sizeof(ResidualEdge) * network->edgeNum);
  ResidualEdgePtr edges = (ResidualEdgePtr)(residual + network->vertNum);
  for (int i = 0; i < network->vertNum; i++) {
    residual[i] = NULL;
    for (ListEdgePtr edge = network->vertices[i].outEdges; edge != NULL; edge = edge->next) {
      const ResidualEdgePtr copy = edges++;
      copy->source = i;
      copy->target = edge->target;
      copy->capacity = edge->tail->flow.capacity;
      copy->flow = 0;
      copy->parallel = edge;
      insert(residual + i, copy);
    }
  }
  return residual;
}

/*
 * 广度优先搜索寻找最短路径，
 * 之所以不用贪心寻找可扩容最大的边，
 * 是因为这可能会导致capacity大的边被反复反转，
 * 不如最短路径收敛稳定 O(V * E^2)
 */
static int bfs(const ResidualEdgePtr *residual, const QueuePtr queue, const VertexId source,
               const VertexId sink, ResidualEdgePtr *const preious) {
  queueClear(queue);
  enqueue(queue, source);
  while (!queueEmpty(queue)) {
    const VertexId vertex = *queueFront(queue);
    dequeue(queue);

    for (ResidualEdgePtr edge = residual[vertex]; edge != NULL; edge = edge->next) {
      const VertexId target = edge->target;
      if (preious[target] != NULL) continue;

      preious[target] = edge;
      if (target == sink) return 1;

      enqueue(queue, target);
    }
  }
  return 0;
}

// 寻找路径可调整的flow = min(capacity - flow)
static FlowType pathFlow(const ResidualEdgePtr *previous, const VertexId sink) {
  FlowType flow = UNREACHABLE;
  for (ResidualEdgePtr edge = previous[sink]; edge != NULL; edge = previous[edge->source]) {
    if (edge->capacity - edge->flow < flow) flow = edge->capacity - edge->flow;
  }
  return flow;
}

FlowType EdmondsKarpMaxFlow(const ListGraph *network, const VertexId source, const VertexId sink) {
  Queue queue;
  queueInit(&queue, network->vertNum);
  ResidualEdgePtr *residual = createResidualNetwork(network);
  ResidualEdgePtr *previous = malloc(sizeof(ListEdgePtr) * network->vertNum);

  FlowType maxFlow = 0;
  while (1) {
    memset(previous, 0, sizeof(ListEdgePtr) * network->vertNum);
    if (!bfs(residual, &queue, source, sink, previous)) break;

    const FlowType flow = pathFlow(previous, sink);
    for (ResidualEdgePtr edge = previous[sink], prev; edge != NULL; edge = prev) {
      prev = previous[edge->source];
      edge->flow += flow;

      // 如果edge是正向的，则flow的增加是同向的；否则相反
      if (edge->target == edge->parallel->target) {
        edge->parallel->tail->flow.current += flow;
      } else {
        edge->parallel->tail->flow.current -= flow;
      }

      if (edge->flow == edge->capacity) {
        // 若残余网络的边的flow满容，则反转，
        // 视作原网络边可释放的flow
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
  free(previous);
  queueFreeData(&queue);
  return maxFlow;
}