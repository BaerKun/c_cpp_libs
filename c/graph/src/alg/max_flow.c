#include "graph/iter.h"
#include "private/graph_detail.h"
#include "private/queue.h"
#include "private/view.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
  GraphId src, sink;
  GraphView *residual;
  GraphIter *iter;
  GraphId *pred;
  const FlowType *cap;
  FlowType *curr;
  FlowType *flow;
} Package;

/*
 * 广度优先搜索寻找最短路径，
 * 之所以不用贪心寻找可扩容最大的边，
 * 是因为这可能会导致capacity大的边被反复反转，
 * 不如最短路径收敛稳定 O(V * E^2)
 */
static GraphBool bfs(const Package *pkg, GraphQueue *const queue) {
  GraphId did, eid, to;
  graphQueueClear(queue);
  graphQueuePush(queue, pkg->src);
  while (!graphQueueEmpty(queue)) {
    const GraphId from = graphQueuePop(queue);

    while (graphIterNextDirect(pkg->iter, from, &did)) {
      forward(pkg->residual, did, &eid, &to);
      if (pkg->pred[to] != INVALID_ID || to == pkg->src) continue;

      pkg->pred[to] = did;

      if (to == pkg->sink) return GRAPH_TRUE;
      graphQueuePush(queue, to);
    }
  }
  return GRAPH_FALSE;
}

// 寻找路径可调整的flow = min(capacity - flow)
static FlowType pathFlow(const Package *pkg) {
  FlowType flow = UNREACHABLE;
  GraphId eid, from;
  for (GraphId did = pkg->pred[pkg->sink]; did != INVALID_ID;
       did = pkg->pred[from]) {
    backward(pkg->residual, did, &eid, &from);
    if (flow > pkg->cap[eid] - pkg->curr[eid]) {
      flow = pkg->cap[eid] - pkg->curr[eid];
    }
  }
  return flow;
}

static void reverse(const GraphView *const residual, const GraphId did,
                    const GraphId from, const GraphId to) {
  GraphId *predNext =
      graphFind(residual->edgeNext, residual->edgeHead + from, did);
  graphUnlink(residual->edgeNext, predNext);
  graphInsertEdge(residual, to, REVERSE(did));
}

static void update(const Package *pkg, const FlowType step) {
  GraphId eid, from, to = pkg->sink;
  for (GraphId did = pkg->pred[to]; did != INVALID_ID; did = pkg->pred[from]) {
    backward(pkg->residual, did, &eid, &from);
    pkg->curr[eid] += step;

    // 如果edge是正向的，则flow的增加是同向的；否则相反
    if (did & 1)
      pkg->flow[eid] -= step;
    else
      pkg->flow[eid] += step;

    if (pkg->curr[eid] == pkg->cap[eid]) {
      // 若残余网络的边的flow满容，则反转，
      // 视作原网络边可释放的flow
      pkg->curr[eid] = 0;
      reverse(pkg->residual, did, from, to);
    }
    to = from;
  }
  graphIterResetEdge(pkg->iter, INVALID_ID);
}

FlowType EdmondsKarpMaxFlow(const Graph *network, const FlowType capacity[],
                            FlowType flow[], const GraphId source,
                            const GraphId sink) {
  const GraphView *view = VIEW(network);
  GraphView *residual = graphViewReserveEdge(view, GRAPH_FALSE);
  graphViewCopyEdge(view, residual);

  GraphIter *iter = graphIterFromView(residual);
  GraphQueue *queue = graphNewQueue(network->vertNum);
  GraphId *pred = malloc(view->vertRange * sizeof(GraphId));
  FlowType *curr = calloc(view->edgeRange, sizeof(FlowType));
  memset(flow, 0, view->edgeRange * sizeof(FlowType));

  const Package pkg = {source, sink,     residual, iter,
                       pred,   capacity, curr,     flow};

  FlowType maxFlow = 0;
  while (1) {
    memset(pred, INVALID_ID, view->vertRange * sizeof(GraphId));
    if (!bfs(&pkg, queue)) break;

    const FlowType step = pathFlow(&pkg);
    update(&pkg, step);
    maxFlow += step;
  }

  free(pred);
  free(curr);
  free(residual);
  graphIterRelease(iter);
  graphQueueRelease(queue);
  return maxFlow;
}