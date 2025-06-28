#ifndef GRAPH_EULER_PATH_H
#define GRAPH_EULER_PATH_H

#include "graph/graph.h"

#define LINK_NODE_DATA_TYPE GraphId
#include "link_node.h"

void EulerCircuit(const Graph *graph, LinkNodePtr *path, GraphId src);

void EulerPath(const Graph *graph, LinkNodePtr *path, GraphId src, GraphId dst);

#endif // GRAPH_EULER_PATH_H