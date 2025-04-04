#include "adjacency_list/Euler_path.h"
#include <stdio.h>

typedef struct {
    LinkNodePtr path;
    VertexId source;
} Argument;

#define STACK_DATA_TYPE Argument

#include "stack.h"

typedef struct {
    EdgePtr *availableEdges;
    VertexId tmpdst;
} Package;

// availableEdges[v] 顶点v的可用边链表，充当递归时的“全局变量”
static EdgePtr *getAvailableEdges(const GraphPtr graph) {
    EdgePtr *availableEdges = malloc(graph->vertexNum * sizeof(void *));

    for (VertexId v = 0; v < graph->vertexNum; v++)
        availableEdges[v] = graph->vertices[v].outEdges;

    return availableEdges;
}

static EdgePtr getEdge(EdgePtr *availableEdge) {
    EdgePtr edge;
    for (edge = *availableEdge; edge && !edge->enable; edge = edge->next) {
        // 使用完重置
        edge->enable = 1;
    }
    if (edge == NULL) {
        *availableEdge = NULL;
        return NULL;
    }

    *availableEdge = edge->next;

    if (edge->reverse != NULL)
        edge->reverse->enable = 0;

    return edge;
}

static int EulerCircuitHelper(Package *package, const LinkNodePtr path, const VertexId source) {
    while (1) {
        const EdgePtr edge = getEdge(package->availableEdges + source);
        if (edge == NULL)
            break;

        nodePushBack(path, edge->target);

        if (!EulerCircuitHelper(package, path->next, edge->target))
            return 0;

        package->tmpdst = source;
    }

    int i = source == package->tmpdst;
    return i;
}

static int EulerPath_stack(EdgePtr *availableEdges, const LinkNodePtr path, const VertexId src,
                           VertexId dst, const int edgeNum) {
    Stack stack;
    stackInit(&stack, edgeNum);

    int success = 0;
    Argument arg = {path, src}; // 当前函数参数

    while (1) {
        const EdgePtr edge = getEdge(availableEdges + arg.source);
        if (edge == NULL) {
            if (arg.source != dst)
                break;

            if(stack.top == 0) {
                success = 1;
                break;
            }

            // 模拟函数返回
            stackPop(&stack, &arg);

            dst = arg.source;
            continue;
        }
        nodePushBack(arg.path, edge->target);

        // 模拟函数调用
        stackPush(&stack, arg);
        arg = (Argument) {arg.path->next, edge->target};
    }
    stackFreeData(&stack);
    return success;
}

static inline int
EulerPath_recursive(EdgePtr *availableEdges, const LinkNodePtr path, const VertexId src, const VertexId dst) {
    Package package = {availableEdges, dst};
    return EulerCircuitHelper(&package, path, src);
}

void EulerPath(const GraphPtr graph, const LinkNodePtr path, const VertexId src, const VertexId dst) {
    EdgePtr *availableEdges = getAvailableEdges(graph);
    path->next = NULL;
    path->data = src;

    if(availableEdges == NULL)
        return;

    if (!
            //EulerPath_recursive(availableEdges, path, src, dst)
            EulerPath_stack(availableEdges, path, src, dst, graph->edgeNum)
            ) {
        puts("EulerPath: No Solution!\n");
        nodeClear(&path->next);
    }
    free(availableEdges);
}

void EulerCircuit(const GraphPtr graph, const LinkNodePtr path, const VertexId source) {
    EulerPath(graph, path, source, source);
}
