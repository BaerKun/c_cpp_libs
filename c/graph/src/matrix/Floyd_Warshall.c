#include "graph/matrix/Floyd_Warshall.h"

/*
 * Floyd-Warshall算法
 * 不限正负权
 * 原理：对路径经过的顶点动态规划
 */

void FloydWarshallWeightedPath(WeightType **weight, const GraphSize vertNum,
                               GraphId **path, WeightType **sum) {
  for (GraphId i = 0; i < vertNum; i++) {
    for (GraphId j = 0; j < vertNum; j++) {
      sum[i][j] = weight[i][j];
      path[i][j] = j;
    }
  }
  for (GraphId middle = 0; middle < vertNum; ++middle) {
    for (GraphId source = 0; source < vertNum; ++source) {
      if (sum[source][middle] == UNREACHABLE) continue;

      for (GraphId target = 0; target < vertNum; ++target) {
        if (sum[middle][target] == UNREACHABLE) continue;

        if (sum[source][middle] + sum[middle][target] < sum[source][target]) {
          sum[source][target] = sum[source][middle] + sum[middle][target];
          path[source][target] = middle;
        }
      }
    }
  }
}