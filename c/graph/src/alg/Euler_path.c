#include "graph/iter.h"
#include "graph/linked_list.h"
#include "private/graph_detail.h"
#include <stdlib.h>

// 递归实现
typedef struct {
  GraphIter *iter;
  GraphBool *visited;
  GraphId dfsDst; // 当前深度优先搜索的目标顶点
} Package;        // 递归全局量

static GraphBool getTargetEdge(const Package *pkg, const GraphId from,
                               GraphId *id, GraphId *to) {
  while (graphIterNextEdge(pkg->iter, from, id, to)) {
    if (!pkg->visited[*id]) {
      pkg->visited[*id] = 1;
      return GRAPH_TRUE;
    }
  }
  return GRAPH_FALSE;
}

static GraphBool EulerPath_recursive(Package *pkg, GraphLinkedNode **const pred,
                                     const GraphId from) {
  GraphId id, to;
  while (1) {
    if (!getTargetEdge(pkg, from, &id, &to)) break;
    GraphLinkedNode *const path = graphLinkedInsert(pred, id);
    if (!EulerPath_recursive(pkg, &path->next, to)) return GRAPH_FALSE;
    pkg->dfsDst = from;
  }
  return (GraphBool)(from == pkg->dfsDst);
}

// 栈实现
typedef struct {
  GraphLinkedNode **pred;
  GraphId from;
} Argument; // 入栈量，与递归相对

static GraphBool EulerPath_stack(Package *pkg, GraphLinkedNode **const head,
                                 const GraphId src, const GraphSize edgeNum) {
  Argument *const stack = malloc(edgeNum * sizeof(Argument));
  Argument *ptr = stack; // 当前"函数"参数
  GraphBool success = GRAPH_FALSE;
  *ptr = (Argument){head, src};

  GraphId id, to;
  while (1) {
    if (!getTargetEdge(pkg, ptr->from, &id, &to)) {
      if (ptr->from != pkg->dfsDst) break;

      // 结束
      if (ptr-- == stack) {
        success = GRAPH_TRUE;
        break;
      }

      pkg->dfsDst = ptr->from;
      continue;
    }
    GraphLinkedNode *const path = graphLinkedInsert(ptr->pred, id);

    // 调用
    *++ptr = (Argument){&path->next, to};
  }

  free(stack);
  return success;
}

void EulerPath(const Graph *const graph, GraphLinkedNode **const path,
               const GraphId src, const GraphId dst) {
  const GraphView *view = VIEW(graph);
  Package pkg;
  pkg.iter = graphIterFromView(view);
  pkg.visited = calloc(view->vertRange, sizeof(GraphBool));
  pkg.dfsDst = dst;

  // clang-format off
  if (!
    // EulerPath_recursive(&pkg, path, src)
    EulerPath_stack(&pkg, path, src, graph->edgeNum)
  ) {
    // clang-format on
    graphLinkedClear(path);
  }

  free(pkg.visited);
  graphIterRelease(pkg.iter);
}

void EulerCircuit(const Graph *const graph, GraphLinkedNode **const path,
                  const GraphId src) {
  EulerPath(graph, path, src, src);
}
