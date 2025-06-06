#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include "share/type.h"

typedef struct MatrixVertex_ MatrixVertex, *MatrixVertexPtr;
typedef struct MatrixEdge_ MatrixEdge, *MatrixEdgePtr;
typedef struct MatrixGraph_ MatrixGraph, *MatrixGraphPtr;

struct MatrixEdge_ {
  VertexId path;
  WeightType weight;
  EdgeData data;
};

struct MatrixVertex_{
  VertexData data;
};

struct MatrixGraph_ {
  int capacity;
  int edgeNum;
  int vertexNum;
  MatrixVertex *vertices;
  MatrixEdge **edges;
};

MatrixGraphPtr graphInit(int capacity, int vertexNum);

void graphDestroy(MatrixGraphPtr graph);

void graphAddEdge(MatrixGraphPtr graph, VertexId source, VertexId target, EdgeData data);

void **SquareMatrix(int rowCols, int sizeOfElem);

#endif // GRAPH_GRAPH_H
