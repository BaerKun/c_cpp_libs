#include "graph/adj_list/find_articulation.h"
#include <stdlib.h>

typedef struct VertexAttribute_ VertexAttribute;
struct VertexAttribute_ {
  GraphId id;
  GraphBool visited;
  GraphId preorder; // dfs中第一次访问节点的序数
  GraphId lowest;   // 该点所在的所有圈的所有顶点中最小的序数（一个点也视作圈）
  VertexAttribute *pred;
};

typedef struct {
  VertexAttribute *vertices;
  GraphEdgePtr *adjList;
  GraphLinkedPath **arts;
  GraphId topo;
} Package;

static void findArticulationStep(Package *package, VertexAttribute *vertex) {
  GraphBool isArt = vertex->pred != NULL ? 1 : 0; // 排除根节点，单独处理

  vertex->visited = 1;
  vertex->lowest = vertex->preorder = package->topo++;
  for (GraphEdgePtr edge = package->adjList[vertex->id]; edge;
       edge = edge->next) {
    VertexAttribute *target = package->vertices + edge->to;

    if (!target->visited) {
      target->pred = vertex;
      findArticulationStep(package, target);

      // 若target所在的圈不包含vertex,则vertex为割点
      // 使用isArt，只添加一次
      if (target->lowest >= vertex->preorder && !isArt) {
        isArt = 1;
        graphPathInsert(package->arts, vertex->id);
      }

      // 递归更新lowest
      if (target->lowest < vertex->lowest) vertex->lowest = target->lowest;
    }
    /*
     * 排除反向边；
     * 若出现访问过的点，说明有圈，
     * 因为单向DFS在无圈图中的遍历是拓扑排序的；
     * 更新lowest
     */
    else if (target != vertex->pred && target->preorder < vertex->lowest) {
      vertex->lowest = target->preorder;
    }
  }
}

void graphFindArticulation(const Graph *const graph,
                           GraphLinkedPath **articulations) {
  VertexAttribute *vertices = malloc(graph->vertCap * sizeof(VertexAttribute));
  for (GraphId i = 0; i < graph->vertCap; i++) {
    vertices[i].id = i;
    vertices[i].visited = 0;
    vertices[i].pred = NULL;
  }

  Package package = {vertices, graph->adjList, articulations, 0};
  findArticulationStep(&package, vertices);

  // 若根节点有两个及以上的子树，则为割点
  unsigned children = 0;
  for (GraphEdgePtr adj = graph->adjList[0]; adj; adj = adj->next) {
    if (vertices[adj->to].pred == vertices && ++children == 2) {
      graphPathInsert(articulations, 0);
      break;
    }
  }
  free(vertices);
}
