#include "adjacency_list/find_articulation.h"
#include <stdlib.h>


typedef struct VertexArg {
    VertexId id;
    int visited;
    int preorder; // dfs中第一次访问节点的序数
    int lowest; // 该点所在的所有圈的所有顶点中最小的序数（一个点也视作圈）
    struct VertexArg *parent;
    EdgePtr outEdges;
} VertexArg;

typedef struct {
    int topo;
    VertexArg *vertices;
    LinkNode **arts;
} Package;

static void findArticulationStep(Package *package, VertexArg *vertex) {
    int isArt = 0;

    vertex->visited = 1;
    vertex->lowest = vertex->preorder = package->topo++;
    for (EdgePtr edge = vertex->outEdges; edge; edge = edge->next) {
        VertexArg *target = package->vertices + edge->target;

        if (!target->visited) {
            target->parent = vertex;
            findArticulationStep(package, target);

            // 若target所在的圈不包含vertex,则vertex为割点
            // 使用isArt，只添加一次
            if (target->lowest >= vertex->preorder && isArt == 0) {
                isArt = 1;
                nodeInsert(package->arts, vertex->id);
            }

            // 递归更新lowest
            if (target->lowest < vertex->lowest)
                vertex->lowest = target->lowest;
        }
        /*
         * 排除反向边；
         * 若出现访问过的点，说明有圈，
         * 因为单向DFS在无圈图中的遍历是拓扑排序的；
         * 更新lowest
         */
        else if (vertex->parent != target &&
                   target->preorder < vertex->lowest) {
            vertex->lowest = target->preorder;
        }
    }
}

void graphFindArticulation(const GraphPtr graph, LinkNode **articulations) {
    VertexArg *vertices = malloc(sizeof(VertexArg) * graph->vertexNum);
    if (vertices == NULL)
        return;

    for (int i = 0; i < graph->vertexNum; i++) {
        vertices[i].id = i;
        vertices[i].visited = 0;
        vertices[i].parent = NULL;
        vertices[i].outEdges = graph->vertices[i].outEdges;
    }

    Package package = {0, vertices, articulations};
    findArticulationStep(&package, vertices);
    free(vertices);
}
