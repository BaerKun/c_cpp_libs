#ifndef GRAPH_EULER_PATH_H
#define GRAPH_EULER_PATH_H

#include "graph/linked_path.h"

void EulerCircuit(const Graph *graph, GraphLinkedPath **path, GraphId src);

void EulerPath(const Graph *graph, GraphLinkedPath **path, GraphId src,
               GraphId dst);

#endif // GRAPH_EULER_PATH_H