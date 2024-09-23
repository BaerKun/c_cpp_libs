#ifndef GRAPH_MIN_SPANNING_TREE_H
#define GRAPH_MIN_SPANNING_TREE_H

#include "graph.h"

void PrimMinSpanningTree(GraphPtr graph, VertexId parent[], VertexId root);

void printTree(GraphPtr graph, VertexId parent[], VertexId root);

#endif //GRAPH_MIN_SPANNING_TREE_H
