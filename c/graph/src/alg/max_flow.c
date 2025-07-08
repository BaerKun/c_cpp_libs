#include "graph/iter.h"
#include "private/graph_detail.h"
#include "private/queue.h"
#include "private/view.h"
#include <stdlib.h>
#include <string.h>

static void reverse(const GraphView *const residual, const GraphId id,
                    const GraphId from, const GraphId to) {
  GraphId *predNext =
      graphFind(residual->edgeNext, residual->edgeHead + from, id);
  graphUnlink(residual->edgeNext, predNext);
  graphInsert(residual->edgeNext, residual->edgeHead + to, REVERSE(id));
}

// 残余网络
static void *residualNetworkInit(const GraphView *network,
                                 GraphView *residual) {
  const GraphSize vertRange = network->vertRange;
  const GraphSize edgeRange = network->edgeRange;
  residual->directed = network->directed;
  residual->vertHead = network->vertHead;
  residual->vertNext = network->vertNext;

  const GraphSize edgeNextSize =
      sizeof(GraphId) * (network->directed ? edgeRange : 2 * edgeRange);
  void *buff = malloc(vertRange * sizeof(GraphId) + edgeNextSize);
  residual->edgeHead = buff;
  memcpy(residual->edgeHead, network->edgeHead,
         network->edgeRange * sizeof(GraphId));

  residual->edgeNext = buff + vertRange;
  memcpy(residual->edgeNext, network->edgeNext, edgeNextSize);
  return buff;
}

/*
 * 广度优先搜索寻找最短路径，
 * 之所以不用贪心寻找可扩容最大的边，
 * 是因为这可能会导致capacity大的边被反复反转，
 * 不如最短路径收敛稳定 O(V * E^2)
 */
static GraphBool bfs(GraphIter *iter, GraphQueue *const queue,
                     GraphId predEdge[], GraphId predVert[],
                     const GraphId source, const GraphId sink) {
  GraphId id, to;
  graphQueueClear(queue);
  graphQueuePush(queue, source);
  while (!graphQueueEmpty(queue)) {
    const GraphId from = graphQueuePop(queue);

    while (graphIterNextEdge(iter, from, &id, &to)) {
      if (predEdge[to] != INVALID_ID || to == source) continue;

      predEdge[to] = id;
      predVert[id] = from;

      if (to == sink) return 1;
      graphQueuePush(queue, to);
    }
  }
  return 0;
}

// 寻找路径可调整的flow = min(capacity - flow)
static FlowType pathFlow(const GraphId predEdge[], const GraphId predVert[],
                         const FlowType capacity[], const FlowType current[],
                         const GraphId sink) {
  FlowType flow = UNREACHABLE;
  for (GraphId edge = predEdge[sink]; edge != INVALID_ID;
       edge = predEdge[predVert[edge]]) {
    if (flow > capacity[edge] - current[edge]) {
      flow = capacity[edge] - current[edge];
    }
  }
  return flow;
}

FlowType EdmondsKarpMaxFlow(const Graph *network, const FlowType capacity[],
                            FlowType flow[], const GraphId source,
                            const GraphId sink) {
  const GraphView *view = VIEW(network);
  GraphView residual;
  void *buff = residualNetworkInit(view, &residual);
  GraphQueue *queue = graphNewQueue(network->vertNum);
  GraphIter *iter = graphIterFromView(&residual);
  GraphId *predEdge = malloc(view->edgeRange * sizeof(GraphId));
  GraphId *predVert = malloc(view->vertRange * sizeof(GraphId));
  GraphId *current = malloc(view->edgeRange * sizeof(FlowType));
  memset(current, 0, view->edgeRange * sizeof(FlowType));
  memset(flow, 0, view->edgeRange * sizeof(FlowType));

  FlowType maxFlow = 0;
  while (1) {
    memset(predEdge, INVALID_ID, view->vertRange * sizeof(GraphId));
    if (!bfs(iter, queue, predEdge, predVert, source, sink)) break;

    const FlowType stepFlow =
        pathFlow(predEdge, predVert, capacity, current, sink);
    for (GraphId edge = predEdge[sink], from, to = sink; edge != INVALID_ID;
         edge = predEdge[from]) {
      from = predVert[edge];
      current[edge] += stepFlow;

      // 如果edge是正向的，则flow的增加是同向的；否则相反
      flow[edge] += edge /**/ ? -stepFlow : stepFlow;

      if (current[edge] == capacity[edge]) {
        // 若残余网络的边的flow满容，则反转，
        // 视作原网络边可释放的flow
        current[edge] = 0;
        reverse(&residual, edge, from, to);
      }
      to = from;
    }
    maxFlow += stepFlow;
  }

  free(buff);
  free(predEdge);
  free(predVert);
  free(current);
  graphIterRelease(iter);
  graphQueueRelease(queue);
  return maxFlow;
}