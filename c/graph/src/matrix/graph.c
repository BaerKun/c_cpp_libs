#include "matrix/graph.h"
#include <stdlib.h>

MatrixGraphPtr createGraph(const int capacity, const int vertexNum) {
  const MatrixGraphPtr graph = malloc(sizeof(MatrixGraph));
  if (graph == NULL) return NULL;

  graph->capacity = capacity;
  graph->edgeNum = 0;
  graph->vertexNum = vertexNum;
  graph->vertices = malloc(sizeof(MatrixVertex) * capacity);
  graph->edges = (MatrixEdge **)SquareMatrix(capacity, sizeof(MatrixEdge));

  for (int i = 0; i < capacity; ++i) {
    for (int j = 0; j < capacity; ++j) {
      graph->edges[i][j].path = -1;
      graph->edges[i][j].weight = UNREACHABLE;
    }
  }

  return graph;
}

void graphDestroy(const MatrixGraphPtr graph) {
  free(graph->vertices);
  free(graph->edges);
  free(graph);
}

void graphAddEdge(const MatrixGraphPtr graph, const VertexId source, const VertexId target,
                  const EdgeData data) {
  graph->edges[source][target].data = data;
  graph->edgeNum++;
}

void **SquareMatrix(const int rowCols, const int sizeOfElem) {
  const int sizeOfRow = sizeOfElem * rowCols;
  void **matrix = malloc((sizeof(void *) + sizeOfRow) * rowCols);
  void *row = (void *)(matrix + rowCols);
  for (int i = 0; i < rowCols; ++i) {
    matrix[i] = row;
    row += sizeOfRow;
  }
  return matrix;
}