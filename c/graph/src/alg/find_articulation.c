#include "private/graph_detail.h"
#include "graph/iter.h"
#include "graph/linked_path.h"
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
  GraphIter *iter;
  VertexAttribute *vertices;
  GraphLinkedPath **arts;
  GraphId topo;
} Package;

static void findArticulationStep(Package *pkg, VertexAttribute *vertex) {
  // 排除根节点，单独处理
  GraphBool isArt = vertex->pred != NULL ? GRAPH_TRUE : GRAPH_FALSE;
  GraphId id, to;

  vertex->visited = 1;
  vertex->lowest = vertex->preorder = pkg->topo++;
  while (graphIterNextEdge(pkg->iter, vertex->id, &id, &to)) {
    VertexAttribute *target = pkg->vertices + to;

    if (!target->visited) {
      target->pred = vertex;
      findArticulationStep(pkg, target);

      // 若target所在的圈不包含vertex,则vertex为割点
      // 使用isArt，只添加一次
      if (target->lowest >= vertex->preorder && !isArt) {
        isArt = 1;
        graphPathInsert(pkg->arts, vertex->id);
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
  const GraphSize vertRange = graph->vertMng.range;
  VertexAttribute *vertices = malloc(vertRange * sizeof(VertexAttribute));
  for (GraphId i = 0; i < vertRange; i++) {
    vertices[i].id = i;
    vertices[i].visited = 0;
    vertices[i].pred = NULL;
  }

  Package pkg;
  pkg.iter = graphGetIter(graph);
  pkg.vertices = vertices;
  pkg.arts = articulations;
  pkg.topo = 0;

  GraphId root, id, to;
  graphIterCurr(pkg.iter, &root, &id, &to);
  findArticulationStep(&pkg, vertices + root);

  // 若根节点有两个及以上的子树，则为割点
  unsigned children = 0;
  graphIterResetEdge(graph, pkg.iter, root);
  while (graphIterNextEdge(pkg.iter, root, &id, &to)) {
    if (vertices[to].pred == vertices && ++children == 2) {
      graphPathInsert(articulations, 0);
      break;
    }
  }
  free(vertices);
  graphIterRelease(pkg.iter);
}
