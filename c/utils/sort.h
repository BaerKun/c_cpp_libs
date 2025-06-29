#ifndef SORT_H
#define SORT_H

#ifndef SORT_ELEM_TYPE
#define SORT_ELEM_TYPE int
#endif

#ifndef SORT_LESS_THAN
#define SORT_LESS_THAN(x, y) ((x) < (y))
#endif

#include <stdint.h>

typedef SORT_ELEM_TYPE SortElemType_;

void sort(SortElemType_ *array, uint64_t size);

void insertSort(SortElemType_ *array, uint64_t size);

void shellSort(SortElemType_ *array, uint64_t size);

void heapSort(SortElemType_ *array, uint64_t size);

void mergeSort(SortElemType_ *array, uint64_t size);

void quickSort(SortElemType_ *array, uint64_t size);

void quickSelect(SortElemType_ *array, uint64_t size, uint64_t number);

void bucketSort(SortElemType_ *array, uint64_t size);

#endif // SORT_H
