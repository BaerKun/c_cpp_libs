#include "graph/matrix/matrix.h"
#include <stdlib.h>

void **matrixCreate(const GraphSize rowCols, const GraphSize sizeOfElem) {
  const GraphSize sizeOfRow = sizeOfElem * rowCols;
  void **matrix = malloc((sizeof(void *) + sizeOfRow) * rowCols);
  void *row = matrix + rowCols;
  for (int i = 0; i < rowCols; ++i) {
    matrix[i] = row;
    row += sizeOfRow;
  }
  return matrix;
}
