#include "graph/adj_list/Euler_path.h"
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

static int EulerRecursiveStep(Package *package, LinkNodePtr *const path,
                              const GraphId src) {
  while (1) {
    const GraphEdgePtr edge =
        getTargetEdge(package->iter + src, package->visited);
    if (edge == NULL) break;

    nodeInsert(path, edge->id);

    if (!EulerRecursiveStep(package, &(*path)->next, edge->to)) return 0;

    package->dfsDst = src;
  }

  return src == package->dfsDst;
}

static inline int EulerPath_recursive(GraphEdgePtr *iter, GraphBool *visited,
                                      LinkNodePtr *const path, const GraphId src,
                                      const GraphId dst) {
  Package package = {iter, visited, dst};
  return EulerRecursiveStep(&package, path, src);
}

// 栈实现
typedef struct {
  LinkNodePtr *path;
  GraphId src;
} Argument; // 入栈量，与递归相对
#define STACK_DATA_TYPE Argument
#include "stack.h"

static int EulerPath_stack(GraphEdgePtr *iter, GraphBool *visited,
                           LinkNodePtr *const path, const GraphId src,
                           GraphId dst, const GraphSize edgeNum) {
  Stack stack;
  stackInit(&stack, (int)edgeNum);

  int success = 0;
  Argument arg = {path, src}; // 当前"函数"参数

  while (1) {
    const GraphEdgePtr edge = getTargetEdge(iter + arg.src, visited);
    if (edge == NULL) {
      if (arg.src != dst) break;

      // 结束
      if (stackEmpty(&stack)) {
        success = 1;
        break;
      }

      // 返回
      arg = *stackTop(&stack);
      stackPop(&stack);

      dst = arg.src;
      continue;
    }
    nodeInsert(arg.path, edge->id);

    // 调用
    stackPush(&stack, arg);
    arg = (Argument){&(*arg.path)->next, edge->to};
  }

  stackFreeData(&stack);
  return success;
}

void EulerPath(const Graph *const graph, LinkNodePtr *const path,
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
    nodeClear(path);
  }

  free(iter);
  free(visited);
}

void EulerCircuit(const Graph *const graph, LinkNodePtr *const path,
                  const GraphId src) {
  EulerPath(graph, path, src, src);
}
