#include "adjacency_list/Euler_path.h"


// 递归实现
typedef struct {
    EdgePtr *unvisitedEdges;
    VertexId dfsDst; // 当前深度优先搜索的目标顶点
} Package; // 全局量

static EdgePtr *getUnvisitedEdges(const GraphPtr graph) {
    EdgePtr *unvisitedEdges = malloc(graph->vertexNum * sizeof(EdgePtr));
    for (VertexId v = 0; v < graph->vertexNum; v++)
        unvisitedEdges[v] = graph->vertices[v].outEdges;
    return unvisitedEdges;
}

static EdgePtr getTargetEdge(EdgePtr *unvisitedEdge) {
    EdgePtr edge;
    for (edge = *unvisitedEdge; edge && !edge->flag; edge = edge->next) {
        // 使用完重置
        edge->flag = 1;
    }

    // 更新
    if (edge == NULL) {
        *unvisitedEdge = NULL;
        return NULL;
    }
    *unvisitedEdge = edge->next;

    if (edge->reverse != NULL)
        edge->reverse->flag = 0; // 用作懒惰删除
    return edge;
}

static int EulerCircuitStep(Package *package, const LinkNodePtr path, const VertexId src) {
    while (1) {
        const EdgePtr edge = getTargetEdge(package->unvisitedEdges + src);
        if (edge == NULL)
            break;

        nodePushBack(path, edge->target);

        if (!EulerCircuitStep(package, path->next, edge->target))
            return 0;

        package->dfsDst = src;
    }

    return src == package->dfsDst;
}

static inline int
EulerPath_recursive(EdgePtr *availableEdges, const LinkNodePtr path, const VertexId src, const VertexId dst) {
    Package package = {availableEdges, dst};
    return EulerCircuitStep(&package, path, src);
}


// 栈实现
typedef struct {
    LinkNodePtr path;
    VertexId src;
} Argument; // 入栈量，与递归相对
#define STACK_DATA_TYPE Argument
#include "stack.h"

static int EulerPath_stack(EdgePtr *availableEdges, const LinkNodePtr path, const VertexId src,
                           VertexId dst, const int edgeNum) {
    Stack stack;
    stackInit(&stack, edgeNum);

    int success = 0;
    Argument arg = {path, src}; // 当前函数参数

    while (1) {
        const EdgePtr edge = getTargetEdge(availableEdges + arg.src);
        if (edge == NULL) {
            if (arg.src != dst)
                break;

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
        nodePushBack(arg.path, edge->target);

        // 调用
        stackPush(&stack, arg);
        arg = (Argument){arg.path->next, edge->target};
    }

    stackFreeData(&stack);
    return success;
}


void EulerPath(const GraphPtr graph, LinkNodePtr *const path, const VertexId src, const VertexId dst) {
    EdgePtr *unvisitedEdges = getUnvisitedEdges(graph);
    if (unvisitedEdges == NULL)
        return;

    *path = newNode(src);
    if (!
        // EulerPath_recursive(unvisitedEdges, path, src, dst)
        EulerPath_stack(unvisitedEdges, *path, src, dst, graph->edgeNum)
    ) {
        nodeClear(path);
    }

    free(unvisitedEdges);
}

void EulerCircuit(const GraphPtr graph, LinkNodePtr *const path, const VertexId src) {
    EulerPath(graph, path, src, src);
}
