#include "graph/iter.h"
#include "private/stack.h"
#include "private/view.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
  GraphIter *iter;
  GraphStack *stack;
  GraphBool *flag;
  GraphId *connectionId;
  GraphId counter;
} Package;

static void findSccForward(Package *pkg, const GraphId from) {
  GraphId did, eid, to;
  pkg->flag[from] = 1;
  while (graphIterNextDirect(pkg->iter, from, &did)) {
    parse(pkg->iter->view, did, &eid, &to);
    if (!pkg->flag[to]) findSccForward(pkg, to);
    graphInsertEdge(pkg->iter->view, to, did); // 边转向
  }
  graphStackPush(pkg->stack, from);
}

static void findSccBackward(Package *pkg, const GraphId from) {
  GraphId did, eid, to;
  pkg->connectionId[from] = pkg->counter;
  pkg->flag[from] = 0;
  while (graphIterNextDirect(pkg->iter, from, &did)) {
    parse(pkg->iter->view, did, &eid, &to);
    if (pkg->flag[to]) findSccBackward(pkg, to);
    graphInsertEdge(pkg->iter->view, to, did); // 转回来
  }
}

static inline void resetHead(const GraphView *view) {
  memset(view->edgeHead, INVALID_ID, view->vertRange * sizeof(GraphId));
}

static inline void graphReverse(const GraphView *view) {
  const GraphEndpoint *end = view->endpts + view->edgeRange;
  for (GraphEndpoint *ptr = view->endpts; ptr != end; ++ptr) {
    const GraphId tmp = ptr->to;
    ptr->to = ptr->from;
    ptr->from = tmp;
  }
}

void graphFindScc(const Graph *graph, GraphId connectionId[]) {
  const GraphView *view = VIEW(graph);
  GraphStack *stack = graphNewStack(graph->vertNum);
  GraphBool *flag = calloc(view->vertRange, sizeof(GraphBool));
  memset(connectionId, INVALID_ID, view->vertRange * sizeof(GraphId));
  Package pkg = {graphIterFromView(view), stack, flag, connectionId, 0};

  // 正序
  GraphId from;
  resetHead(view);
  while (graphIterNextVert(pkg.iter, &from)) {
    if (flag[from] == 0) findSccForward(&pkg, from);
  }

  // 逆序
  graphIterResetEdge(pkg.iter, INVALID_ID);
  resetHead(view);
  graphReverse(view);
  while (!graphStackEmpty(stack)) {
    const GraphId vert = graphStackPop(stack);
    if (flag[vert] == 1) findSccBackward(&pkg, vert);
    ++pkg.counter;
  }

  free(flag);
  graphIterRelease(pkg.iter);
  graphStackRelease(stack);
}
