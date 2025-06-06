#ifndef GRAPH_EULER_PATH_H
#define GRAPH_EULER_PATH_H

#include "graph.h"

#define LINK_NODE_DATA_TYPE VertexId
#include "link_node.h"

void EulerCircuit(ListGraphPtr graph, LinkNodePtr *path, VertexId src);

void EulerPath(ListGraphPtr graph, LinkNodePtr *path, VertexId src, VertexId dst);

#endif // GRAPH_EULER_PATH_H