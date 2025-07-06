#include "graph/iter.h"
#include "private/graph_detail.h"
#include "private/stack.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
  const GraphManager *edge;
  GraphStack *stack;
  GraphBool *flag;
  GraphId *connectionId;
  GraphId counter;
} Package;

static void findSccForward(Package *pkg, const GraphId from) {
  // 拷贝，清空，以便之后加入反向边
  pkg->edge->iterHead[from] = INVALID_ID;

  GraphId id, to;
  pkg->flag[from] = 1;
  while (graphIterNextEdge(pkg->iter, from, &id, &to)) {
    if (!pkg->flag[to]) findSccForward(pkg, to);

    // 边转向
    next = edge->next;
    edge->to = from;
    edgeInsert(pkg->adjList + to, edge);
  }
  graphStackPush(pkg->stack, from);
}

static void findSccBackward(Package *pkg, const GraphId from) {
  pkg->edge->iterHead[from] = INVALID_ID;

  GraphId id, to;
  pkg->connectionId[from] = pkg->counter;
  pkg->flag[from] = 0;
  while (graphIterNextEdge(pkg->iter, from, &id, &to)) {
    if (pkg->flag[to]) findSccBackward(pkg, to);

    // 转回来
    next = edge->next;
    edge->to = from;
    edgeInsert(pkg->adjList + to, edge);
  }
}

void graphFindScc(const Graph *graph, GraphId connectionId[]) {
  GraphStack *stack = graphNewStack(graph->vertNum);
  GraphBool *flag = calloc(graph->vertCap, sizeof(GraphBool));
  Package pkg = {&graph->edgeMng, stack, flag, connectionId, 0};
  memset(connectionId, 255, graph->vertCap * sizeof(GraphId));

  // 正序
  GraphId from;
  while (graphIterNextVert(pkg.iter, &from)) {
    if (flag[from] == 0) findSccForward(&pkg, from);
  }

  // 逆序
  graphIterResetEdge(graph, pkg.iter, INVALID_ID);
  while (!graphStackEmpty(stack)) {
    const GraphId vert = graphStackPop(stack);
    if (flag[vert] == 1) findSccBackward(&pkg, vert);
    ++pkg.counter;
  }

  free(flag);
  graphIterRelease(pkg.iter);
  graphStackRelease(stack);
}
