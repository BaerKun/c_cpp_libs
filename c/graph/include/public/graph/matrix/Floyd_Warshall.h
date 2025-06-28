#ifndef GRAPH_ALL_PAIRS_WEIGHTED_PATH_H
#define GRAPH_ALL_PAIRS_WEIGHTED_PATH_H

#include "graph/type.h"

// 不限正负权值
void FloydWarshallWeightedPath(WeightType **weight, GraphSize vertNum,
                               GraphId **path, WeightType **distance);

#endif // GRAPH_ALL_PAIRS_WEIGHTED_PATH_H
