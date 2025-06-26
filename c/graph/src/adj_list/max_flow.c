#include "graph/adj_list/max_flow.h"
#include "queue.h"
#include <stdlib.h>
#include <string.h>

typedef struct ResidualEdge_ ResidualEdge, *ResidualEdgePtr;
struct ResidualEdge_ {
  GraphId id;
  GraphBool reverse;
  GraphId from, to;
  FlowType capacity, current;
  ResidualEdgePtr *prevNext;
  ResidualEdgePtr next;
};

static void unlink(const ResidualEdgePtr edge) {
  const ResidualEdgePtr next = edge->next;
  *edge->prevNext = next;
  if (next != NULL) next->prevNext = edge->prevNext;
}

static void insert(ResidualEdgePtr *const prevNext,
                   const ResidualEdgePtr edge) {
  const ResidualEdgePtr next = *prevNext;
  edge->next = next;
  *prevNext = edge;
  edge->prevNext = prevNext;
  if (next != NULL) next->prevNext = &edge->next;
}

static void reverse(ResidualEdgePtr *const residual,
                    const ResidualEdgePtr edge) {
  const GraphId from = edge->from;
  edge->reverse = !edge->reverse;
  edge->from = edge->to;
  edge->to = from;
  unlink(edge);
  insert(residual + edge->from, edge);
}

// 残余网络
static ResidualEdgePtr *createResidualNetwork(const Graph *network,
                                              const FlowType capacity[]) {
  ResidualEdgePtr *residual =
      malloc(network->vertCap * sizeof(ResidualEdgePtr) +
             network->edgeCap * sizeof(ResidualEdge));
  ResidualEdgePtr edges = (ResidualEdgePtr)(residual + network->vertCap);

  for (GraphId from = 0; from < network->vertCap; from++) {
    residual[from] = NULL;
    for (GraphEdgePtr edge = network->adjList[from]; edge != NULL;
         edge = edge->next) {
      const ResidualEdgePtr copy = edges++;
      copy->id = edge->id;
      copy->reverse = 0;
      copy->from = from;
      copy->to = edge->to;
      copy->capacity = capacity[edge->id];
      copy->current = 0;
      insert(residual + from, copy);
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
static int bfs(const ResidualEdgePtr *residual, const QueuePtr queue,
               ResidualEdgePtr pred[], const GraphId source,
               const GraphId sink) {
  queueClear(queue);
  enqueue(queue, source);
  while (!queueEmpty(queue)) {
    const GraphId vertex = *queueFront(queue);
    dequeue(queue);

    for (ResidualEdgePtr edge = residual[vertex]; edge != NULL;
         edge = edge->next) {
      const GraphId target = edge->to;
      if (pred[target] != NULL) continue;

      pred[target] = edge;
      if (target == sink) return 1;

      enqueue(queue, target);
    }
  }
  return 0;
}

// 寻找路径可调整的flow = min(capacity - flow)
static FlowType pathFlow(const ResidualEdgePtr *pred, const GraphId sink) {
  FlowType flow = UNREACHABLE;
  for (ResidualEdgePtr edge = pred[sink]; edge != NULL;
       edge = pred[edge->from]) {
    if (edge->capacity - edge->current < flow) {
      flow = edge->capacity - edge->current;
    }
  }
  return flow;
}

FlowType EdmondsKarpMaxFlow(const Graph *network, const FlowType capacity[],
                            const GraphId source, const GraphId sink) {
  Queue queue;
  queueInit(&queue, network->vertNum);
  ResidualEdgePtr *residual = createResidualNetwork(network, capacity);
  ResidualEdgePtr *pred = malloc(sizeof(GraphEdgePtr) * network->vertCap);
  FlowType *current = calloc(network->edgeCap, sizeof(FlowType));

  FlowType maxFlow = 0;
  while (1) {
    memset(pred, 0, sizeof(GraphEdgePtr) * network->vertCap);
    if (!bfs(residual, &queue, pred, source, sink)) break;

    const FlowType flow = pathFlow(pred, sink);
    for (ResidualEdgePtr edge = pred[sink], prev; edge != NULL; edge = prev) {
      prev = pred[edge->from];
      edge->current += flow;

      // 如果edge是正向的，则flow的增加是同向的；否则相反
      current[edge->id] += edge->reverse ? -flow : flow;

      if (edge->current == edge->capacity) {
        // 若残余网络的边的flow满容，则反转，
        // 视作原网络边可释放的flow
        edge->current = 0;
        reverse(residual, edge);
      }
    }
    maxFlow += flow;
  }

  free(residual);
  free(pred);
  free(current);
  queueFreeData(&queue);
  return maxFlow;
}