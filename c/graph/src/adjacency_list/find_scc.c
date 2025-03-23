#include "adjacency_list/find_scc.h"
#include "adjacency_list/edge_list.h"

typedef struct VertexArg {
    char visitedOnce;
    int *number;
    EdgePtr *outEdge;
} VertexArg;

#define STACK_ELEMENT_TYPE VertexArg *
#include <stack.h>

typedef struct {
    VertexArg *vertices;
    StackPtr stack;
    int counter;
} Package;

static void findSccForward(Package *package, VertexArg *vertex) {
    // 拷贝，清空，以便之后加入反向边
    EdgePtr edge = *vertex->outEdge;
    *vertex->outEdge = NULL;

    vertex->visitedOnce = 1;
    for (EdgePtr nextEdge; edge; edge = nextEdge) {
        VertexArg *adjacentVertex = package->vertices + edge->target;
        if (!adjacentVertex->visitedOnce)
            findSccForward(package, adjacentVertex);

        // 边转向，加入thisEdge->outEdges链表
        nextEdge = edge->next;
        edge->target = (VertexId)(vertex - package->vertices);
        edgeInsert(adjacentVertex->outEdge, edge);
    }

    stackPush(package->stack, vertex);
}

static void findSccBackward(Package *package, VertexArg *vertex) {
    EdgePtr edge = *vertex->outEdge;
    *vertex->outEdge = NULL;

    *vertex->number = package->counter;
    vertex->visitedOnce = 0;
    for (EdgePtr nextEdge; edge; edge = nextEdge) {
        VertexArg *adjacentVertex = package->vertices + edge->target;
        if (adjacentVertex->visitedOnce)
            findSccBackward(package, adjacentVertex);

        // 转回来
        nextEdge = edge->next;
        edge->target = (VertexId)(vertex - package->vertices);
        edgeInsert(adjacentVertex->outEdge, edge);
    }
}

void graphFindScc(const GraphPtr graph, int number[]) {
    VertexArg *vertices = malloc(graph->vertexNum * sizeof(VertexArg));
    const StackPtr stack = newStack(graph->vertexNum);
    Package package = {vertices, stack, 0};

    for (VertexId vertex = 0; vertex < graph->vertexNum; vertex++) {
        vertices[vertex].visitedOnce = 0;
        vertices[vertex].number = number + vertex;
        number[vertex] = -1;
        vertices[vertex].outEdge = &graph->vertices[vertex].outEdges;
    }

    // 正序
    const VertexArg *end = vertices + graph->vertexNum;
    for (VertexArg *vertex = vertices; vertex != end; vertex++) {
        if (vertex->visitedOnce == 0)
            findSccForward(&package, vertex);
    }

    // 逆序
    while (stack->top != 0) {
        VertexArg *vertex = stackPop(stack);
        if (vertex->visitedOnce == 1)
            findSccBackward(&package, vertex);
        ++package.counter;
    }

    free(vertices);
}
