#include "graph/iter.h"
#include "graph/linked_list.h"
#include "private/graph_detail.h"
#include <stdlib.h>

typedef struct VertexAttribute_ Vertex;
struct VertexAttribute_ {
  GraphBool visited;
  GraphId preorder; // dfs中第一次访问节点的序数
  GraphId lowest;   // 该点所在的所有圈的所有顶点中最小的序数（一个点也视作圈）
  Vertex *pred;
};

typedef struct {
  GraphIter *iter;
  Vertex *vertices;
  GraphLinkedNode **arts;
  GraphId topo;
} Package;

static void findArticulationStep(Package *pkg, const GraphId from) {
  // 排除根节点，单独处理
  Vertex *vertex = pkg->vertices + from;
  GraphBool isArt = vertex->pred != NULL ? GRAPH_TRUE : GRAPH_FALSE;
  GraphId id, to;

  vertex->visited = 1;
  vertex->lowest = vertex->preorder = pkg->topo++;
  while (graphIterNextEdge(pkg->iter, from, &id, &to)) {
    Vertex *adjacent = pkg->vertices + to;

    if (!adjacent->visited) {
      adjacent->pred = vertex;
      findArticulationStep(pkg, to);

      // 若target所在的圈不包含vertex,则vertex为割点
      // 使用isArt，只添加一次
      if (adjacent->lowest >= vertex->preorder && !isArt) {
        isArt = 1;
        graphLinkedInsert(pkg->arts, from);
      }

      // 递归更新lowest
      if (adjacent->lowest < vertex->lowest) vertex->lowest = adjacent->lowest;
    }
    /*
     * 排除反向边；
     * 若出现访问过的点，说明有圈，
     * 因为单向DFS在无圈图中的遍历是拓扑排序的；
     * 更新lowest
     */
    else if (adjacent != vertex->pred && adjacent->preorder < vertex->lowest) {
      vertex->lowest = adjacent->preorder;
    }
  }
}

void graphFindArticulation(const Graph *const graph,
                           GraphLinkedNode **articulations) {
  const GraphView *view = VIEW(graph);
  Vertex *vertices = calloc(view->vertRange, sizeof(Vertex));

  Package pkg = {graphIterFromView(view), vertices, articulations, 0};
  const GraphId root = pkg.iter->vertCurr;
  findArticulationStep(&pkg, root);

  // 若根节点有两个及以上的子树，则为割点
  GraphId id, to;
  unsigned children = 0;
  graphIterResetEdge(pkg.iter, root);
  while (graphIterNextEdge(pkg.iter, root, &id, &to)) {
    if (vertices[to].pred == vertices && ++children == 2) {
      graphLinkedInsert(articulations, 0);
      break;
    }
  }
  free(vertices);
  graphIterRelease(pkg.iter);
}
