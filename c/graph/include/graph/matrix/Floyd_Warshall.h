#ifndef GRAPH_ALL_PAIRS_WEIGHTED_PATH_H
#define GRAPH_ALL_PAIRS_WEIGHTED_PATH_H

#include "graph/share/type.h"

// 不限正负权值
void FloydWarshallWeightedPath(WeightType **weight, GraphSize vertNum,
                               GraphId **path, WeightType **sum);

#endif // GRAPH_ALL_PAIRS_WEIGHTED_PATH_H
