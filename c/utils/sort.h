#ifndef SORT_H
#define SORT_H

#ifndef SORT_DATA_TYPE
#define SORT_DATA_TYPE int
#endif

#ifndef SORT_LESS_THAN
#define SORT_LESS_THAN(x, y) ((x) < (y))
#endif

void sort(SORT_DATA_TYPE *array, int size);

void insertSort(SORT_DATA_TYPE *array, int size);

void shellSort(SORT_DATA_TYPE *array, int size);

void heapSort(SORT_DATA_TYPE *array, int size);

void mergeSort(SORT_DATA_TYPE *array, int size);

void quickSort(SORT_DATA_TYPE *array, int size);

void quickSelect(SORT_DATA_TYPE *array, int size, int number);

void bucketSort(SORT_DATA_TYPE *array, int size);

#endif // SORT_H
