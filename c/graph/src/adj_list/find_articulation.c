#include "graph/adj_list/find_articulation.h"
#include <stdlib.h>

typedef struct VertexAttribute_ VertexAttribute;
struct VertexAttribute_ {
  GraphId id;
  GraphBool visited;
  GraphId preorder; // dfs中第一次访问节点的序数
  GraphId lowest;   // 该点所在的所有圈的所有顶点中最小的序数（一个点也视作圈）
  VertexAttribute *parent;
};

typedef struct {
  GraphId topo;
  VertexAttribute *vertices;
  GraphEdgePtr *adjList;
  LinkNode **arts;
} Package;

static void findArticulationStep(Package *package, VertexAttribute *vertex) {
  GraphBool isArt = 0;

  vertex->visited = 1;
  vertex->lowest = vertex->preorder = package->topo++;
  for (GraphEdgePtr edge = package->adjList[vertex->id]; edge;
       edge = edge->next) {
    VertexAttribute *target = package->vertices + edge->to;

    if (!target->visited) {
      target->parent = vertex;
      findArticulationStep(package, target);

      // 若target所在的圈不包含vertex,则vertex为割点
      // 使用isArt，只添加一次
      if (target->lowest >= vertex->preorder && !isArt &&
          vertex->parent != NULL) { // 排除根节点，单独处理
        isArt = 1;
        nodeInsert(package->arts, vertex->id);
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
    else if (target != vertex->parent && target->preorder < vertex->lowest) {
      vertex->lowest = target->preorder;
    }
  }
}

void graphFindArticulation(const Graph *const graph,
                           LinkNodePtr *articulations) {
  VertexAttribute *vertices = malloc(sizeof(VertexAttribute) * graph->vertCap);
  if (vertices == NULL) return;

  for (GraphId i = 0; i < graph->vertCap; i++) {
    vertices[i].id = i;
    vertices[i].visited = 0;
    vertices[i].parent = NULL;
  }

  Package package = {0, vertices, graph->adjList, articulations};
  findArticulationStep(&package, vertices);

  unsigned children = 0;
  for (GraphEdgePtr adj = graph->adjList[0]; adj; adj = adj->next) {
    if (vertices[adj->to].parent == vertices && ++children == 2) {
      nodeInsert(articulations, 0);
      break;
    }
  }
  free(vertices);
}
