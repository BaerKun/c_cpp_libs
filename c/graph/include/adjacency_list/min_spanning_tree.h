#ifndef GRAPH_MIN_SPANNING_TREE_H
#define GRAPH_MIN_SPANNING_TREE_H

#include "graph.h"

void PrimMinSpanningTree(ListGraphPtr graph, VertexId parent[], VertexId root);

void printTree(ListGraphPtr graph, VertexId parent[], VertexId root);

#endif // GRAPH_MIN_SPANNING_TREE_H
