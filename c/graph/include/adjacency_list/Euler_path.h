#ifndef GRAPH_EULER_PATH_H
#define GRAPH_EULER_PATH_H

#include "graph.h"
#include "node.h"

void EulerCircuit(GraphPtr graph, NodePtr path, VertexId source);

void EulerPath(GraphPtr graph, NodePtr path, VertexId src, VertexId dst);

#endif //GRAPH_EULER_PATH_H
