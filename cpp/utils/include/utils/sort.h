#ifndef SORT_H
#define SORT_H

#include <functional>

#define CallbackType const std::function<bool(const DataType_&, const DataType_&)>&

namespace cpp_libs {
    template<typename DataType_>
    void sort(DataType_ *array, int size, CallbackType compare);

    template<typename DataType_>
    void select(DataType_ *array, int size, int number, CallbackType compare);

    namespace sort_detail {
        template<typename DataType_>
        void insert(DataType_ *array, int size, CallbackType compare);

        template<typename DataType_>
        void shell(DataType_ *array, int size, CallbackType compare);

        template<typename DataType_>
        void heap(DataType_ *array, int size, CallbackType compare);

        template<typename DataType_>
        void merge(DataType_ *array, int size, CallbackType compare);

        template<typename DataType_>
        void quick(DataType_ *array, int size, CallbackType compare);

        // 只支持整数
        template<typename DataType_>
        void bucket(DataType_ *array, int size);
    }
}

#include "sort.hpp"

#undef CallbackType
#endif //SORT_H
