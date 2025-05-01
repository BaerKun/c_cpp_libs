#ifndef SORT_HPP
#define SORT_HPP

#include <iostream>

namespace cpp_libs {
    namespace sort_detail {
        template<typename DataType_>
        static inline void swap(DataType_ *const x, DataType_ *const y) {
            const DataType_ z = *x;
            *x = *y;
            *y = z;
        }

        template<typename DataType_>
        static void mergeLeftRight(DataType_ *array, DataType_ *tmpArray, CallbackType compare,
                                   const int left, const int center, const int right) {
            int i = left;
            int j = center + 1;
            int out = left;

            while (true) {
                if (compare(array[i], array[j])) {
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

        template<typename DataType_>
        static void percolateDown(DataType_ *prev, CallbackType compare, const int top, const int last) {
            const DataType_ topValue = prev[top];

            int father = top;
            for (int child; (child = father << 1) <= last; father = child) {
                // 因为堆排序时，堆顶元素从后往前存放，所以要交换比较数
                if (child != last && compare(prev[child], prev[child + 1]))
                    child++;
                if (compare(topValue, prev[child]))
                    prev[father] = prev[child];
                else
                    break;
            }
            prev[father] = topValue;
        }

        template<typename DataType_>
        static DataType_ median(DataType_ *array, CallbackType compare, const int left, const int right) {
            const int center = (left + right) >> 1;

            if (compare(array[center], array[left]))
                swap(array + left, array + center);
            if (compare(array[right], array[left]))
                swap(array + left, array + right);
            if (compare(array[right], array[center]))
                swap(array + center, array + right);

            swap(array + center, array + right - 1);

            return array[right - 1];
        }

        template<typename DataType_>
        static int quickBody(DataType_ *array, CallbackType compare, const int left, const int right) {
            DataType_ const pivot = median(array, compare, left, right);
            int i = left, j = right - 1;

            while (true) {
                while (compare(array[++i], pivot)) {
                }
                while (compare(pivot, array[--j])) {
                }
                if (i < j)
                    swap(array + i, array + j);
                else
                    break;
            }
            swap(array + i, array + right - 1);

            return i;
        }

        template<typename DataType_>
        static void bucketSortHelper(DataType_ *iArray, DataType_ *oArray, int *buckets, const int size,
                                     const int shift) {
            DataType_ tmp1, tmp2;

            for (int i = 0; i < 16; i++)
                buckets[i] = 0;
            for (int i = 0; i < size; i++) {
                ++buckets[(iArray[i] >> shift) & 15];
            }

            tmp1 = buckets[0];
            buckets[0] = 0;
            for (int i = 1; i < 16; i++) {
                tmp2 = buckets[i];
                buckets[i] = tmp1 + buckets[i - 1];
                tmp1 = tmp2;
            }
            for (int i = 0; i < size; i++) {
                oArray[buckets[(iArray[i] >> shift) & 15]++] = iArray[i];
            }
        }

        template<typename DataType_>
        void insert(DataType_ *array, const int size, CallbackType compare) {
            for (int i = 1, j; i < size; i++) {
                const DataType_ tmp = array[i];
                for (j = i; j && compare(tmp, array[j - 1]); j--)
                    array[j] = array[j - 1];
                array[j] = tmp;
            }
        }

        template<typename DataType_>
        void shell(DataType_ *array, const int size, CallbackType compare) {
            int delta;
            DataType_ tmp;
            for (delta = 1; delta < size; delta = delta << 1 | 1) {
            }
            for (delta >>= 1; delta; delta >>= 1) {
                for (int j, i = delta; i < size; i++) {
                    tmp = array[i];
                    for (j = i; j >= delta && compare(tmp, array[j - delta]); j -= delta)
                        array[j] = array[j - delta];
                    array[j] = tmp;
                }
            }
        }

        template<typename DataType_>
        void heap(DataType_ *array, const int size, CallbackType compare) {
            DataType_ *prev = array - 1;

            for (int i = size >> 1; i; --i)
                percolateDown(prev, compare, i, size);

            for (int i = size - 1; i > 0; --i) {
                swap(array + i, array);
                percolateDown(prev, compare, 1, i);
            }
        }

        template<typename DataType_>
        void merge(DataType_ *array, const int size, CallbackType compare) {
            auto *tmpArray = new DataType_[size];
            if (!tmpArray)
                return;

            std::function<void(int, int)> recursion;
            recursion = [array, tmpArray, &compare, &recursion](const int left, const int right) {
                if (left < right) {
                    const int center = (left + right) >> 1;
                    recursion(left, center);
                    recursion(center + 1, right);
                    mergeLeftRight(array, tmpArray, compare, left, center, right);
                }
            };
            recursion(0, size - 1);
            delete[] tmpArray;
        }

        template<typename DataType_>
        void quick(DataType_ *array, const int size, CallbackType compare) {
            std::function<void(int, int)> recursion;
            recursion = [array, &compare, &recursion](const int left, const int right) {
                if (left + 10 < right) {
                    const int i = quickBody(array, compare, left, right);
                    recursion(left, i - 1);
                    recursion(i + 1, right);
                } else
                    insert(array + left, right - left + 1, compare);
            };
            recursion(0, size - 1);
        }

        template<typename DataType_>
        void bucket(DataType_ *array, const int size) {
            // 位移超过数据位数时，<< >>的结果是未定义的
            const int SHIFT_LIMIT = sizeof(DataType_) * 8;
            int shift = 0, buckets[16];
            auto *tmpArray = new DataType_[size];
            if (!tmpArray)
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

            delete[] tmpArray;
        }
    }

    template<typename DataType_>
    void sort(DataType_ *array, const int size, CallbackType compare) {
        sort_detail::quick(array, size, compare);
    }

    template<typename DataType_>
    void select(DataType_ *array, const int size, const int number, CallbackType compare) {
        std::function<void(int, int)> recursion;
        // number从0开始
        recursion = [array, number, &compare, &recursion](const int left, const int right) {
            if (left + 10 < right) {
                const int i = quickBody(array, compare, left, right);
                if (number < i)
                    recursion(left, i - 1);
                else if (number > i)
                    recursion(i + 1, right);
            } else
                insert(array + left, right - left + 1, compare);
        };

        recursion(array, 0, size - 1);
    }
}

#endif
