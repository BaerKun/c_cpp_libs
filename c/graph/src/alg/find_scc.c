#include "graph/iter.h"
#include "private/graph_detail.h"
#include "private/stack.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
  GraphIter *iter;
  GraphStack *stack;
  GraphBool *flag;
  GraphId *connectionId;
  const GraphManager *edge;
  GraphId counter;
} Package;

static void graphManagerInsert(const GraphManager *mng, const GraphId from,
                               const GraphId id) {
  mng->next[id] = mng->iterHead[from];
  mng->iterHead[from] = id;
}

static void findSccForward(Package *pkg, const GraphId from) {
  GraphId id, to;
  pkg->flag[from] = 1;
  while (graphIterNextEdge(pkg->iter, from, &id, &to)) {
    if (!pkg->flag[to]) findSccForward(pkg, to);

    // 边转向
    graphManagerInsert(pkg->edge, to, ~id);
  }
  graphStackPush(pkg->stack, from);
}

static void findSccBackward(Package *pkg, const GraphId from) {
  GraphId id, to;
  pkg->connectionId[from] = pkg->counter;
  pkg->flag[from] = 0;
  while (graphIterNextEdge(pkg->iter, from, &id, &to)) {
    if (pkg->flag[to]) findSccBackward(pkg, to);

    // 转回来
    graphManagerInsert(pkg->edge, to, ~id);
  }
}

void graphFindScc(const Graph *graph, GraphId connectionId[]) {
  const GraphSize vertRange = graph->vertMng.range;
  GraphStack *stack = graphNewStack(graph->vertNum);
  GraphBool *flag = calloc(vertRange, sizeof(GraphBool));
  Package pkg = {graphGetIter(graph), stack,           flag,
                 connectionId,        &graph->edgeMng, 0};
  memset(connectionId, INVALID_ID, vertRange * sizeof(GraphId));

  // 正序
  GraphId from;
  memset(graph->edgeMng.iterHead, INVALID_ID, vertRange * sizeof(GraphId));
  while (graphIterNextVert(pkg.iter, &from)) {
    if (flag[from] == 0) findSccForward(&pkg, from);
  }

  // 逆序
  graphIterResetEdge(graph, pkg.iter, INVALID_ID);
  memset(graph->edgeMng.iterHead, INVALID_ID, vertRange * sizeof(GraphId));
  while (!graphStackEmpty(stack)) {
    const GraphId vert = graphStackPop(stack);
    if (flag[vert] == 1) findSccBackward(&pkg, vert);
    ++pkg.counter;
  }

  free(flag);
  graphIterRelease(pkg.iter);
  graphStackRelease(stack);
}
