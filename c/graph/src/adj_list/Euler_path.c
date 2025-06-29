#include "graph/adj_list/Euler_path.h"
#include <stdlib.h>
#include <string.h>

// 递归实现
typedef struct {
  GraphEdgePtr *iter;
  GraphBool *visited;
  GraphId dfsDst; // 当前深度优先搜索的目标顶点
} Package;        // 递归全局量

static GraphEdgePtr getTargetEdge(GraphEdgePtr *const iter,
                                  GraphBool *const visited) {
  GraphEdgePtr edge = *iter;
  while (edge && visited[edge->id]) edge = edge->next;

  // 更新
  if (edge == NULL) {
    *iter = NULL;
    return NULL;
  }
  *iter = edge->next;
  visited[edge->id] = 1;
  return edge;
}

static int EulerRecursiveStep(Package *package, GraphLinkedPath **const pred,
                              const GraphId src) {
  while (1) {
    const GraphEdgePtr edge =
        getTargetEdge(package->iter + src, package->visited);
    if (edge == NULL) break;

    GraphLinkedPath *const path = graphPathInsert(pred, edge->id);

    if (!EulerRecursiveStep(package, &path->next, edge->to)) return 0;

    package->dfsDst = src;
  }

  return src == package->dfsDst;
}

static inline int EulerPath_recursive(GraphEdgePtr *iter, GraphBool *visited,
                                      GraphLinkedPath **const path,
                                      const GraphId src, const GraphId dst) {
  Package package = {iter, visited, dst};
  return EulerRecursiveStep(&package, path, src);
}

// 栈实现
typedef struct {
  GraphLinkedPath **pred;
  GraphId src;
} Argument; // 入栈量，与递归相对

static GraphBool EulerPath_stack(GraphEdgePtr *iter, GraphBool *visited,
                                 GraphLinkedPath **const head,
                                 const GraphId src, GraphId dst,
                                 const GraphSize edgeNum) {
  Argument *const stack = malloc(edgeNum * sizeof(Argument));
  Argument *ptr = stack; // 当前"函数"参数
  GraphBool success = 0;
  *ptr = (Argument){head, src};

  while (1) {
    const GraphEdgePtr edge = getTargetEdge(iter + ptr->src, visited);
    if (edge == NULL) {
      if (ptr->src != dst) break;

      // 结束
      if (ptr-- == stack) {
        success = 1;
        break;
      }

      dst = ptr->src;
      continue;
    }
    GraphLinkedPath *const path = graphPathInsert(ptr->pred, edge->id);

    // 调用
    *++ptr = (Argument){&path->next, edge->to};
  }

  free(stack);
  return success;
}

void EulerPath(const Graph *const graph, GraphLinkedPath **const path,
               const GraphId src, const GraphId dst) {
  GraphEdgePtr *iter = malloc(graph->vertCap * sizeof(GraphEdgePtr));
  GraphBool *visited = calloc(graph->edgeCap, sizeof(GraphBool));
  memcpy(iter, graph->adjList, graph->vertCap * sizeof(GraphEdgePtr));

  // clang-format off
  if (!
    // EulerPath_recursive(iter, visited, path, src, dst)
    EulerPath_stack(iter, visited, path, src, dst, graph->edgeNum)
  ) {
    // clang-format on
    graphPathClear(path);
  }

  free(iter);
  free(visited);
}

void EulerCircuit(const Graph *const graph, GraphLinkedPath **const path,
                  const GraphId src) {
  EulerPath(graph, path, src, src);
}
