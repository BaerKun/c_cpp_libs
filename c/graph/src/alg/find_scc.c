#include "private/graph_detail.h"
#include "private/stack.h"
#include "graph/iter.h"
#include <string.h>
#include <stdlib.h>

typedef struct {
  GraphIter iter;
  GraphBool *flag;
  GraphId *connectionId;
  GraphStack *stack;
  GraphId counter;
} Package;

static void findSccForward(Package *package, const GraphId from) {
  // 拷贝，清空，以便之后加入反向边
  GraphEdgePtr edge = package->adjList[from];
  package->adjList[from] = NULL;

  package->flag[from] = 1;
  for (GraphEdgePtr next; edge; edge = next) {
    const GraphId to = edge->to;
    if (!package->flag[to]) findSccForward(package, to);

    // 边转向
    next = edge->next;
    edge->to = from;
    edgeInsert(package->adjList + to, edge);
  }
  graphStackPush(package->stack, from);
}

static void findSccBackward(Package *package, const GraphId from) {
  GraphEdgePtr edge = package->adjList[from];
  package->adjList[from] = NULL;

  package->connectionId[from] = package->counter;
  package->flag[from] = 0;
  for (GraphEdgePtr next; edge; edge = next) {
    const GraphId to = edge->to;
    if (package->flag[to]) findSccBackward(package, to);

    // 转回来
    next = edge->next;
    edge->to = from;
    edgeInsert(package->adjList + to, edge);
  }
}

void graphFindScc(const Graph *graph, GraphId connectionId[]) {
  GraphStack stack;
  graphStackInit(&stack, graph->vertNum);
  GraphBool *flag = calloc(graph->vertCap, sizeof(GraphBool));
  Package package = {graph->adjList, flag, connectionId, &stack, 0};
  memset(connectionId, 255, graph->vertCap * sizeof(GraphId));

  // 正序
  for (GraphId vert = 0; vert != graph->vertCap; ++vert) {
    if (flag[vert] == 0) {
      findSccForward(&package, vert);
    }
  }

  // 逆序
  while (!graphStackEmpty(&stack)) {
    const GraphId vert = graphStackPop(&stack);
    if (flag[vert] == 1) findSccBackward(&package, vert);
    ++package.counter;
  }

  free(flag);
  graphStackRelease(&stack);
}
