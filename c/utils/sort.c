#include "sort.h"
#include <stdlib.h>

static inline void swap(SortElemType_ *const x, SortElemType_ *const y) {
  SortElemType_ const z = *x;
  *x = *y;
  *y = z;
}

void sort(SortElemType_ *const array, const uint64_t size) {
  quickSort(array, size);
}

void insertSort(SortElemType_ *const array, const uint64_t size) {
  for (uint64_t i = 1, j; i < size; i++) {
    SortElemType_ const tmp = array[i];
    for (j = i; j && SORT_LESS_THAN(tmp, array[j - 1]); j--)
      array[j] = array[j - 1];
    array[j] = tmp;
  }
}

void shellSort(SortElemType_ *const array, const uint64_t size) {
  uint64_t j, delta;

  for (delta = 1; delta < size; delta = delta << 1 | 1) {
  }
  for (delta >>= 1; delta; delta >>= 1) {
    for (uint64_t i = delta; i < size; i++) {
      SortElemType_ const tmp = array[i];
      for (j = i; j >= delta && SORT_LESS_THAN(tmp, array[j - delta]);
           j -= delta)
        array[j] = array[j - delta];
      array[j] = tmp;
    }
  }
}

static void percolateDown(SortElemType_ *const prev, uint64_t father,
                          const uint64_t end) {
  SortElemType_ const theTop = prev[father];

  for (uint64_t child; (child = father << 1) <= end; father = child) {
    if (child != end && SORT_LESS_THAN(prev[child], prev[child + 1])) child++;
    if (SORT_LESS_THAN(theTop, prev[child]))
      prev[father] = prev[child];
    else
      break;
  }
  prev[father] = theTop;
}

void heapSort(SortElemType_ *const array, const uint64_t size) {
  SortElemType_ *prev = array - 1;

  for (uint64_t i = size >> 1; i; --i) percolateDown(prev, i, size);

  for (uint64_t i = size - 1; i > 0; --i) {
    swap(array + i, array);
    percolateDown(prev, 1, i);
  }
}

static void mergeLeftRight(SortElemType_ *const array,
                           SortElemType_ *const tmpArray, const uint64_t left,
                           const uint64_t center, const uint64_t right) {
  uint64_t i = left;
  uint64_t j = center + 1;
  uint64_t out = left;

  while (1) {
    if (SORT_LESS_THAN(array[i], array[j])) {
      tmpArray[out++] = array[i];
      if (++i > center) break;
    } else {
      tmpArray[out++] = array[j];
      if (++j > right) break;
    }
  }

  while (i <= center) tmpArray[out++] = array[i++];
  while (j <= right) tmpArray[out++] = array[j++];
  for (i = left; i <= right; ++i) array[i] = tmpArray[i];
}

static void mergeSortStep(SortElemType_ *const array,
                          SortElemType_ *const tmpArray, const uint64_t left,
                          const uint64_t right) {
  if (left < right) {
    const uint64_t center = (left + right) >> 1;
    mergeSortStep(array, tmpArray, left, center);
    mergeSortStep(array, tmpArray, center + 1, right);
    mergeLeftRight(array, tmpArray, left, center + 1, right);
  }
}

void mergeSort(SortElemType_ *const array, const uint64_t size) {
  SortElemType_ *tmpArray = malloc(size * sizeof(SortElemType_));
  if (tmpArray) {
    mergeSortStep(array, tmpArray, 0, size - 1);
    free(tmpArray);
  }
}

static SortElemType_ median(SortElemType_ *const array, const uint64_t left,
                            const uint64_t right) {
  const uint64_t center = (left + right) >> 1;

  if (SORT_LESS_THAN(array[center], array[left]))
    swap(array + left, array + center);
  if (SORT_LESS_THAN(array[right], array[left]))
    swap(array + left, array + right);
  if (SORT_LESS_THAN(array[right], array[center]))
    swap(array + center, array + right);

  swap(array + center, array + right - 1);
  return array[right - 1];
}

static uint64_t quickBody(SortElemType_ *const array, const uint64_t left,
                          const uint64_t right) {
  SortElemType_ const pivot = median(array, left, right);
  uint64_t i = left, j = right - 1;

  while (1) {
    while (SORT_LESS_THAN(array[++i], pivot));
    while (SORT_LESS_THAN(pivot, array[--j]));
    if (i >= j) break;
    swap(array + i, array + j);
  }
  swap(array + i, array + right - 1);
  return i;
}

static void quickSortStep(SortElemType_ *const array, const uint64_t left,
                          const uint64_t right) {
  if (left + 10 < right) {
    const uint64_t i = quickBody(array, left, right);
    quickSortStep(array, left, i - 1);
    quickSortStep(array, i + 1, right);
  } else
    insertSort(array + left, right - left + 1);
}

void quickSort(SortElemType_ *array, const uint64_t size) {
  quickSortStep(array, 0, size - 1);
}

static void quickSelectStep(SortElemType_ *const array, const uint64_t left,
                            const uint64_t right, const uint64_t number) {
  if (left + 10 < right) {
    const uint64_t i = quickBody(array, left, right);
    if (number < i)
      quickSelectStep(array, left, i - 1, number);
    else if (number > i)
      quickSelectStep(array, i + 1, right, number);
  } else
    insertSort(array + left, right - left + 1);
}

void quickSelect(SortElemType_ *const array, const uint64_t size,
                 const uint64_t number) {
  quickSelectStep(array, 0, size - 1, number - 1);
}

static void bucketSortHelper(const SortElemType_ iArray[],
                             SortElemType_ oArray[], uint64_t buckets[],
                             const uint64_t size, const uint64_t shift) {
  for (int i = 0; i < 16; i++) buckets[i] = 0;
  for (int i = 0; i < size; i++) {
    ++buckets[(iArray[i] >> shift) & 15];
  }

  uint64_t tmp1 = buckets[0];
  buckets[0] = 0;
  for (int i = 1; i < 16; i++) {
    uint64_t const tmp2 = buckets[i];
    buckets[i] = tmp1 + buckets[i - 1];
    tmp1 = tmp2;
  }
  for (int i = 0; i < size; i++) {
    oArray[buckets[(iArray[i] >> shift) & 15]++] = iArray[i];
  }
}

void bucketSort(SortElemType_ *const array, const uint64_t size) {
  const uint64_t SHIFT_LIMIT = sizeof(SortElemType_) * 8;
  uint64_t shift = 0, buckets[16];
  SortElemType_ *tmpArray = malloc(size * sizeof(SortElemType_));
  if (tmpArray == NULL) return;

  do {
    bucketSortHelper(array, tmpArray, buckets, size, shift);
    if (buckets[1] == size) {
      for (int i = 0; i < size; i++) array[i] = tmpArray[i];
      break;
    }
    shift += 4;
    bucketSortHelper(tmpArray, array, buckets, size, shift);
    shift += 4;
  } while (buckets[1] != size && shift != SHIFT_LIMIT);

  free(tmpArray);
}
