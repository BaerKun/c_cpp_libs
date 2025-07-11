#include "graph/iter.h"
#include "private/stack.h"
#include "private/view.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
  GraphIter *iter;
  GraphView *reverse;
  GraphStack *stack;
  GraphBool *flag;
  GraphId *connectionId;
  GraphId counter;
} Package;

static void findSccForward(Package *pkg, const GraphId from) {
  GraphId did, eid, to;
  pkg->flag[from] = 1;
  while (graphIterNextDirect(pkg->iter, from, &did)) {
    forward(pkg->iter->view, did, &eid, &to);
    if (!pkg->flag[to]) findSccForward(pkg, to);
    graphInsertEdge(pkg->reverse, to, REVERSE(did)); // 边转向
  }
  graphStackPush(pkg->stack, from);
}

static void findSccBackward(Package *pkg, const GraphId from) {
  GraphId did, eid, to;
  pkg->connectionId[from] = pkg->counter;
  pkg->flag[from] = 0;
  while (graphIterNextDirect(pkg->iter, from, &did)) {
    forward(pkg->iter->view, did, &eid, &to);
    if (pkg->flag[to]) findSccBackward(pkg, to);
  }
}

void graphFindScc(const Graph *graph, GraphId connectionId[]) {
  const GraphView *view = VIEW(graph);
  GraphIter *iter = graphIterFromView(view);
  GraphView *reverse = graphViewReserveEdge(view, GRAPH_TRUE);
  GraphStack *stack = graphNewStack(graph->vertNum);
  GraphBool *flag = calloc(view->vertRange, sizeof(GraphBool));
  Package pkg = {iter, reverse, stack, flag, connectionId, 0};
  memset(reverse->edgeHead, INVALID_ID, view->vertRange * sizeof(GraphId));
  memset(connectionId, INVALID_ID, view->vertRange * sizeof(GraphId));

  // 正序
  GraphId from;
  while (graphIterNextVert(pkg.iter, &from)) {
    if (flag[from] == 0) findSccForward(&pkg, from);
  }

  // 逆序
  iter->view = reverse;
  graphIterResetEdge(pkg.iter, INVALID_ID);
  while (!graphStackEmpty(stack)) {
    const GraphId vert = graphStackPop(stack);
    if (flag[vert] == 1) findSccBackward(&pkg, vert);
    ++pkg.counter;
  }

  free(flag);
  free(reverse);
  graphIterRelease(iter);
  graphStackRelease(stack);
}
