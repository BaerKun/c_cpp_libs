#include "adjacency_list/Euler_path.h"
#include <stdio.h>
#include "adjacency_list/edge_list.h"

typedef struct {
    NodePtr path;
    VertexId source;
} Argument;

#define STACK_ELEMENT_TYPE Argument
#include "stack.h"

typedef struct {
    EdgePtr **availableEdges;
    VertexId tmpdst;
} Package;

// availableEdges[v] 顶点v的可用边链表，充当递归时的“全局变量”
static EdgePtr **getAvailableEdges(const GraphPtr graph) {
    EdgePtr **availableEdges = malloc(graph->vertexNum * sizeof(EdgePtr *));

    for (VertexId v = 0; v < graph->vertexNum; v++)
        availableEdges[v] = &graph->vertices[v].outEdges;

    return availableEdges;
}

static EdgePtr getEdge(EdgePtr *availableEdges[], const VertexId source) {
    const EdgePtr edge = *availableEdges[source];
    if (edge == NULL)
        return NULL;

    availableEdges[source] = &edge->next;
    const VertexId target = edge->target;

    // “删除”反向（target->source）边（如有，则移至availableEdge前一个节点）
    const EdgePtr reverseEdges = edgeUnlinkWithTarget(availableEdges[target], source);
    edgeInsert(availableEdges[target], reverseEdges);
    availableEdges[target] = &reverseEdges->next;
    return edge;
}

static int EulerCircuitHelper(Package *package, const NodePtr path, const VertexId source) {
    while (1) {
        const EdgePtr edge = getEdge(package->availableEdges, source);
        if (edge == NULL)
            break;

        nodeInsert(&path->next, edge->target);

        if (!EulerCircuitHelper(package, path->next, edge->target))
            return 0;
        package->tmpdst = source;
    }

    return source == package->tmpdst;
}

void EulerPath_stack(const GraphPtr graph, const NodePtr path, const VertexId src, VertexId dst) {
    const StackPtr stack = newStack(graph->edgeNum);
    EdgePtr **availableEdges = getAvailableEdges(graph);

    path->element = src;
    path->next = NULL;
    Argument arg = {path, src}; // 当前函数参数
    do {
        const EdgePtr edge = getEdge(availableEdges, arg.source);
        if (edge == NULL) {
            if (arg.source != dst) {
                puts("EulerCircuit: No Circuit\n");
                nodeClear(&path->next);
                break;
            }

            // 模拟函数返回
            arg = stackPop(stack);
            dst = arg.source;
            continue;
        }
        nodeInsert(&arg.path->next, edge->target);

        // 模拟函数调用
        stackPush(stack, arg);
        arg = (Argument){arg.path->next, edge->target};
    } while (stack->top != 0);

    free(availableEdges);
    stackDestroy(stack);
}

static void EulerPath_recursive(const GraphPtr graph, const NodePtr path, const VertexId src, const VertexId dst) {
    EdgePtr **availableEdges = getAvailableEdges(graph);

    path->element = src;
    path->next = NULL;
    Package package = {availableEdges, dst};

    if (!EulerCircuitHelper(&package, path, src)) {
        puts("EulerCircuit: No Circuit\n");
        nodeClear(&path->next);
    }

    free(availableEdges);
}

void EulerPath(const GraphPtr graph, const NodePtr path, const VertexId src, const VertexId dst) {
    EulerPath_recursive(graph, path, src, dst);
}

void EulerCircuit(const GraphPtr graph, const NodePtr path, const VertexId source) {
    EulerPath(graph, path, source, source);
}
