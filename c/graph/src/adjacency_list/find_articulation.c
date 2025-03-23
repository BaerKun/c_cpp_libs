#include "adjacency_list/find_articulation.h"
#include <stdlib.h>

typedef struct VertexArg {
    char visited;
    int preorder; // dfs中第一次访问节点的序数
    int lowest; // 子树所那到达的最低节点的序数
    struct VertexArg *parent;
    EdgePtr outEdges;
} VertexArg;

typedef struct {
    VertexArg *vertices;
    int counter;
    VertexId *outptr;
} Package;

static void findArticulationHelper(Package *package, VertexArg *vertex) {
    vertex->visited = 1;
    vertex->lowest = vertex->preorder = package->counter++;
    for (EdgePtr edge = vertex->outEdges; edge; edge = edge->next) {
        VertexArg *adjacentVertex = package->vertices + edge->target;

        if (!adjacentVertex->visited) {
            adjacentVertex->parent = vertex;
            findArticulationHelper(package, adjacentVertex);

            if (adjacentVertex->lowest >= vertex->preorder) {
                *package->outptr = vertex - package->vertices;
                package->outptr++;
            }

            if (adjacentVertex->lowest < vertex->lowest)
                vertex->lowest = adjacentVertex->lowest;

        } else if (vertex->parent != adjacentVertex &&
                   adjacentVertex->preorder < vertex->lowest) {
            vertex->lowest = adjacentVertex->preorder;
        }
    }
}

void graphFindArticulation(const GraphPtr graph, VertexId outputArray[]) {
    VertexArg *vertices = malloc(sizeof(VertexArg) * graph->vertexNum);
    if (vertices == NULL)
        return;

    for (int i = 0; i < graph->vertexNum; i++) {
        vertices[i].parent = NULL;
        vertices[i].visited = 0;
        vertices[i].outEdges = graph->vertices[i].outEdges;
    }

    Package package = {vertices, 0, outputArray};
    findArticulationHelper(&package, vertices);

    int counter = 0;
    for (EdgePtr edge = vertices->outEdges; edge; edge = edge->next) {
        if (vertices[edge->target].parent != NULL && counter++) // memory == parent
            break;
    }

    if (counter == 1)
        *(package.outptr - 1) = -1;

    free(vertices);
}
