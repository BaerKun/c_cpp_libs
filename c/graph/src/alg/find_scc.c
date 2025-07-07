#include "graph/iter.h"
#include "private/stack.h"
#include "private/utils.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
  GraphIter *iter;
  GraphStack *stack;
  GraphBool *flag;
  GraphId *connectionId;
  GraphId counter;
} Package;

static inline void reverse(const GraphView *view, const GraphId to,
                           const GraphId id) {
  graphInsert(view->edgeNext, view->edgeHead + to, REVERSE(id));
}

static void findSccForward(Package *pkg, const GraphId from) {
  GraphId id, to;
  pkg->flag[from] = 1;
  while (graphIterNextEdge(pkg->iter, from, &id, &to)) {
    if (!pkg->flag[to]) findSccForward(pkg, to);
    reverse(pkg->iter->view, to, id); // 边转向
  }
  graphStackPush(pkg->stack, from);
}

static void findSccBackward(Package *pkg, const GraphId from) {
  GraphId id, to;
  pkg->connectionId[from] = pkg->counter;
  pkg->flag[from] = 0;
  while (graphIterNextEdge(pkg->iter, from, &id, &to)) {
    if (pkg->flag[to]) findSccBackward(pkg, to);
    reverse(pkg->iter->view, to, id); // 转回来
  }
}

static inline void resetHead(const GraphView *view) {
  memset(view->edgeHead, INVALID_ID, view->vertRange * sizeof(GraphId));
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
  while (!graphStackEmpty(stack)) {
    const GraphId vert = graphStackPop(stack);
    if (flag[vert] == 1) findSccBackward(&pkg, vert);
    ++pkg.counter;
  }

  free(flag);
  graphIterRelease(pkg.iter);
  graphStackRelease(stack);
}
