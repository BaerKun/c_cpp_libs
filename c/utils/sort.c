#include "sort.h"
#include <stdlib.h>

void shellSort(SORT_ELEMENT_TYPE *array, int size) {
    int i, j, delta;
    SORT_ELEMENT_TYPE tmp;
    for (delta = 1; delta < size; delta = delta << 1 | 1);
    for (delta >>= 1; delta; delta >>= 1) {
        for (i = delta; i < size; i++) {
            tmp = array[i];
            for (j = i; j >= delta && SORT_LESS_THAN(tmp, array[j - delta]); j -= delta)
                array[j] = array[j - delta];
            array[j] = tmp;
        }
    }
}

static void percolateDown(SORT_ELEMENT_TYPE *prev, int father, int end){
    int child;
    SORT_ELEMENT_TYPE theTop = prev[father];

    for( ; (child = father << 1) <= end; father = child){
        if(child != end && SORT_LESS_THAN(prev[child], prev[child + 1]))
            child++;
        if(SORT_LESS_THAN(theTop, prev[child]))
            prev[father] = prev[child];
        else
            break;
    }
    prev[father] = theTop;
}

void heapSort(SORT_ELEMENT_TYPE *array, int size) {
    int *prev = array - 1;

    for(int i = size >> 1; i; --i)
        percolateDown(prev, i, size);

    for(int i = size - 1; i > 0; --i) {
        swap(array + i, array);
        percolateDown(prev, 1, i);
    }
}

static void merge(SORT_ELEMENT_TYPE *array, SORT_ELEMENT_TYPE *tmpArray, int Lpos, int Rpos, int Rend) {
    int Lend, tmpPos, Lstart;
    Lend = Rpos - 1;
    tmpPos = Lstart = Lpos;

    while (1) {
        if (SORT_LESS_THAN(array[Lpos], array[Rpos])) {
            tmpArray[tmpPos++] = array[Lpos++];
            if (Lpos > Lend)
                break;
        } else {
            tmpArray[tmpPos++] = array[Rpos++];
            if (Rpos > Rend)
                break;
        }
    }

    while (Lpos <= Lend)
        tmpArray[tmpPos++] = array[Lpos++];
    while (Rpos <= Rend)
        tmpArray[tmpPos++] = array[Rpos++];
    for (Lpos = Lstart; Lpos <= Rend; Lpos++)
        array[Lpos] = tmpArray[Lpos];
}

static void mergeSortHelper(SORT_ELEMENT_TYPE *array, int *tmpArray, int left, int right) {
    if (left < right) {
        int center = (left + right) >> 1;
        mergeSortHelper(array, tmpArray, left, center);
        mergeSortHelper(array, tmpArray, ++center, right);
        merge(array, tmpArray, left, center, right);
    }
}

void mergeSort(SORT_ELEMENT_TYPE *array, int size) {
    SORT_ELEMENT_TYPE *tmpArray = malloc(size * sizeof(SORT_ELEMENT_TYPE));
    if (tmpArray) {
        mergeSortHelper(array, tmpArray, 0, size - 1);
        free(tmpArray);
    }
}

void quickSort(SORT_ELEMENT_TYPE *array, int size) {
    quickSortHelper(array, 0, size - 1);
}

static void quickSelectHelper(SORT_ELEMENT_TYPE *array, int left, int right, int number) {
    if (left + 10 < right) {
        int i = quickBody(array, left, right);
        if(number < i)
            quickSelectHelper(array, left, i - 1, number);
        else if(number > i)
            quickSelectHelper(array, i + 1, right, number);
    } else
        insertSort(array + left, right - left + 1);
}

void quickSelect(SORT_ELEMENT_TYPE *array, int size, int number) {
    quickSelectHelper(array, 0, size - 1, number - 1);
}

static void bucketSortHelper(SORT_ELEMENT_TYPE *iArray, SORT_ELEMENT_TYPE *oArray, int size, int *buckets, int time) {
    int i, shift;
    SORT_ELEMENT_TYPE tmp1, tmp2;
    shift = time << 2;

    for (i = 0; i < 16; i++)
        buckets[i] = 0;
    for (i = 0; i < size; i++)
        buckets[iArray[i] >> shift & 15]++;

    tmp1 = buckets[0];
    buckets[0] = 0;
    for (i = 1; i < 16; i++) {
        tmp2 = buckets[i];
        buckets[i] = tmp1 + buckets[i - 1];
        tmp1 = tmp2;
    }
    for (i = 0; i < size; i++) {
        oArray[buckets[iArray[i] >> shift & 15]++] = iArray[i];
    }
}

void bucketSort(SORT_ELEMENT_TYPE *array, int size) {
    int i, time=0, buckets[16];
    SORT_ELEMENT_TYPE *tmpArray = malloc(size * sizeof(SORT_ELEMENT_TYPE));
    if (!tmpArray)
        return;

    do {
        bucketSortHelper(array, tmpArray, size, buckets, time);
        if (buckets[1] == size) {
            for (i = 0; i < size; i++)
                array[i] = tmpArray[i];
            break;
        }
        time++;
        bucketSortHelper(tmpArray, array, size, buckets, time);
        time++;
    } while (buckets[1] != size);

    free(tmpArray);
}
