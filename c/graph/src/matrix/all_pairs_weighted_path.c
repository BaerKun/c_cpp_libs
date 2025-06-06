#include "matrix/all_pairs_weighted_path.h"
#include <stdlib.h>

static WeightType **initDist(const MatrixGraphPtr graph) {
  WeightType **distances = (WeightType **)SquareMatrix(graph->vertexNum, sizeof(WeightType));

  for (VertexId source = 0; source < graph->vertexNum; source++) {
    for (VertexId target = 0; target < graph->vertexNum; target++) {
      distances[source][target] = graph->edges[source][target].weight;
    }
  }
  return distances;
}

/*
 * Floyd-Warshall算法
 * 不限正负权
 * 原理：对路径经过的顶点动态规划
 */

void allPairsWeightedPath(const MatrixGraphPtr graph) {
  WeightType **distances = initDist(graph);

  for (VertexId middle = 0; middle < graph->vertexNum; middle++) {
    for (VertexId source = 0; source < graph->vertexNum; source++) {
      if (distances[source][middle] == UNREACHABLE) continue;

      for (VertexId target = 0; target < graph->vertexNum; target++) {
        if (distances[middle][target] == UNREACHABLE) continue;

        if (distances[source][middle] + distances[middle][target] < distances[source][target]) {
          distances[source][target] = distances[source][middle] + distances[middle][target];

          if (graph->edges[source][middle].weight != UNREACHABLE) {
            graph->edges[source][target].path = middle;
          }
        }
      }
    }
  }
  free(distances);
}