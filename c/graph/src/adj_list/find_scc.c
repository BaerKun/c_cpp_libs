#include "graph/adj_list/find_scc.h"
#include "graph/adj_list/edge.h"

typedef struct VertexArg {
  GraphBool flag;
  GraphId *number;
  GraphEdgePtr *outEdge;
} VertexArg;

#define STACK_DATA_TYPE VertexArg *
#include "stack.h"

typedef struct {
  VertexArg *vertices;
  StackPtr stack;
  GraphId counter;
} Package;

static void findSccForward(Package *package, VertexArg *vertex) {
  // 拷贝，清空，以便之后加入反向边
  GraphEdgePtr edge = *vertex->outEdge;
  *vertex->outEdge = NULL;

  vertex->flag = 1;
  for (GraphEdgePtr nextEdge; edge; edge = nextEdge) {
    VertexArg *adjacentVertex = package->vertices + edge->to;
    if (!adjacentVertex->flag) findSccForward(package, adjacentVertex);

    // 边转向，加入thisEdge->outEdges链表
    nextEdge = edge->next;
    edge->to = (vertex - package->vertices);
    edgeInsert(adjacentVertex->outEdge, edge);
  }

  stackPush(package->stack, vertex);
}

static void findSccBackward(Package *package, VertexArg *vertex) {
  GraphEdgePtr edge = *vertex->outEdge;
  *vertex->outEdge = NULL;

  *vertex->number = package->counter;
  vertex->flag = 0;
  for (GraphEdgePtr nextEdge; edge; edge = nextEdge) {
    VertexArg *adjacentVertex = package->vertices + edge->to;
    if (adjacentVertex->flag) findSccBackward(package, adjacentVertex);

    // 转回来
    nextEdge = edge->next;
    edge->to = (vertex - package->vertices);
    edgeInsert(adjacentVertex->outEdge, edge);
  }
}

void graphFindScc(const Graph *graph, int number[]) {
  VertexArg *vertices = malloc(graph->vertNum * sizeof(VertexArg));
  Stack stack;
  stackInit(&stack, graph->vertNum);
  Package package = {vertices, &stack, 0};

  for (GraphId vertex = 0; vertex < graph->vertNum; vertex++) {
    vertices[vertex].flag = 0;
    vertices[vertex].number = number + vertex;
    number[vertex] = -1;
    vertices[vertex].outEdge = graph->adjList + vertex;
  }

  // 正序
  const VertexArg *end = vertices + graph->vertNum;
  for (VertexArg *vertex = vertices; vertex != end; vertex++) {
    if (vertex->flag == 0) findSccForward(&package, vertex);
  }

  // 逆序
  while (stack.top != 0) {
    VertexArg *vertex = *stackTop(&stack);
    stackPop(&stack);
    if (vertex->flag == 1) findSccBackward(&package, vertex);
    ++package.counter;
  }

  free(vertices);
  stackFreeData(&stack);
}
