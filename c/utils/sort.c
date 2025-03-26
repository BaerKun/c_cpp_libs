#include "sort.h"
#include <stdlib.h>

static inline void swap(SORT_DATA_TYPE *const x, SORT_DATA_TYPE *const y) {
    SORT_DATA_TYPE const z = *x;
    *x = *y;
    *y = z;
}

void sort(SORT_DATA_TYPE *array, const int size) {
    quickSort(array, size);
}

void insertSort(SORT_DATA_TYPE *array, const int size) {
    for (int i = 1, j; i < size; i++) {
        SORT_DATA_TYPE const tmp = array[i];
        for (j = i; j && SORT_LESS_THAN(tmp, array[j - 1]); j--)
            array[j] = array[j - 1];
        array[j] = tmp;
    }
}

void shellSort(SORT_DATA_TYPE *array, const int size) {
    int j, delta;

    for (delta = 1; delta < size; delta = delta << 1 | 1) {
    }
    for (delta >>= 1; delta; delta >>= 1) {
        for (int i = delta; i < size; i++) {
            SORT_DATA_TYPE const tmp = array[i];
            for (j = i; j >= delta && SORT_LESS_THAN(tmp, array[j - delta]); j -= delta)
                array[j] = array[j - delta];
            array[j] = tmp;
        }
    }
}

static void percolateDown(SORT_DATA_TYPE *prev, int father, int end) {
    SORT_DATA_TYPE const theTop = prev[father];

    for (int child; (child = father << 1) <= end; father = child) {
        if (child != end && SORT_LESS_THAN(prev[child], prev[child + 1]))
            child++;
        if (SORT_LESS_THAN(theTop, prev[child]))
            prev[father] = prev[child];
        else
            break;
    }
    prev[father] = theTop;
}

void heapSort(SORT_DATA_TYPE *array, int size) {
    int *prev = array - 1;

    for (int i = size >> 1; i; --i)
        percolateDown(prev, i, size);

    for (int i = size - 1; i > 0; --i) {
        swap(array + i, array);
        percolateDown(prev, 1, i);
    }
}

static void mergeLeftRight(SORT_DATA_TYPE *array, SORT_DATA_TYPE *tmpArray,
                           const int left, const int center, const int right) {
    int i = left;
    int j = center + 1;
    int out = left;

    while (1) {
        if (SORT_LESS_THAN(array[i], array[j])) {
            tmpArray[out++] = array[i];
            if (++i > center)
                break;
        } else {
            tmpArray[out++] = array[j];
            if (++j > right)
                break;
        }
    }

    while (i <= center)
        tmpArray[out++] = array[i++];
    while (j <= right)
        tmpArray[out++] = array[j++];
    for (i = left; i <= right; ++i)
        array[i] = tmpArray[i];
}

static void mergeSortHelper(SORT_DATA_TYPE *array, SORT_DATA_TYPE *tmpArray, const int left, const int right) {
    if (left < right) {
        int center = (left + right) >> 1;
        mergeSortHelper(array, tmpArray, left, center);
        mergeSortHelper(array, tmpArray, ++center, right);
        mergeLeftRight(array, tmpArray, left, center, right);
    }
}

void mergeSort(SORT_DATA_TYPE *array, const int size) {
    SORT_DATA_TYPE *tmpArray = malloc(size * sizeof(SORT_DATA_TYPE));
    if (tmpArray) {
        mergeSortHelper(array, tmpArray, 0, size - 1);
        free(tmpArray);
    }
}

static SORT_DATA_TYPE median(SORT_DATA_TYPE *array, const int left, const int right) {
    const int center = (left + right) >> 1;

    if (SORT_LESS_THAN(array[center], array[left]))
        swap(array + left, array + center);
    if (SORT_LESS_THAN(array[right], array[left]))
        swap(array + left, array + right);
    if (SORT_LESS_THAN(array[right], array[center]))
        swap(array + center, array + right);

    swap(array + center, array + right - 1);

    return array[right - 1];
}

static int quickBody(SORT_DATA_TYPE *array, const int left, const int right) {
    SORT_DATA_TYPE const pivot = median(array, left, right);
    int i = left, j = right - 1;

    while (1) {
        while (SORT_LESS_THAN(array[++i], pivot)){}
        while (SORT_LESS_THAN(pivot, array[--j])){}
        if (i < j)
            swap(array + i, array + j);
        else
            break;
    }
    swap(array + i, array + right - 1);

    return i;
}

static void quickSortHelper(SORT_DATA_TYPE *array, const int left, const int right) {
    if (left + 10 < right) {
        const int i = quickBody(array, left, right);
        quickSortHelper(array, left, i - 1);
        quickSortHelper(array, i + 1, right);
    } else
        insertSort(array + left, right - left + 1);
}

void quickSort(SORT_DATA_TYPE *array, const int size) {
    quickSortHelper(array, 0, size - 1);
}

static void quickSelectHelper(SORT_DATA_TYPE *array, const int left, const int right, const int number) {
    if (left + 10 < right) {
        const int i = quickBody(array, left, right);
        if (number < i)
            quickSelectHelper(array, left, i - 1, number);
        else if (number > i)
            quickSelectHelper(array, i + 1, right, number);
    } else
        insertSort(array + left, right - left + 1);
}

void quickSelect(SORT_DATA_TYPE *array, const int size, const int number) {
    quickSelectHelper(array, 0, size - 1, number - 1);
}

static void bucketSortHelper(SORT_DATA_TYPE iArray[], SORT_DATA_TYPE oArray[],
                             int buckets[], const int size, const int shift){
    for (int i = 0; i < 16; i++)
        buckets[i] = 0;
    for (int i = 0; i < size; i++) {
        ++buckets[(iArray[i] >> shift) & 15];
    }

    SORT_DATA_TYPE tmp1 = buckets[0];
    buckets[0] = 0;
    for (int i = 1; i < 16; i++) {
        SORT_DATA_TYPE const tmp2 = buckets[i];
        buckets[i] = tmp1 + buckets[i - 1];
        tmp1 = tmp2;
    }
    for (int i = 0; i < size; i++) {
        oArray[buckets[(iArray[i] >> shift) & 15]++] = iArray[i];
    }
}
void bucketSort(SORT_DATA_TYPE *array, const int size) {
    const int SHIFT_LIMIT = sizeof(SORT_DATA_TYPE) * 8;
    int shift = 0, buckets[16];
    SORT_DATA_TYPE *tmpArray = malloc(size * sizeof(SORT_DATA_TYPE));
    if (tmpArray == NULL)
        return;

    do {
        bucketSortHelper(array, tmpArray, buckets, size, shift);
        if (buckets[1] == size) {
            for (int i = 0; i < size; i++)
                array[i] = tmpArray[i];
            break;
        }
        shift += 4;
        bucketSortHelper(tmpArray, array, buckets, size, shift);
        shift += 4;
    } while (buckets[1] != size && shift != SHIFT_LIMIT);

    free(tmpArray);
}
