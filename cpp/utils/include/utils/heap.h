#ifndef HEAP_H
#define HEAP_H

#include <stdexcept>

namespace cpp_libs {
    template<typename DataType_>
    class Heap {
    public:
        explicit Heap(const int capacity) : data_(new DataType_[capacity]), capacity_(capacity), deleteData(true) {
            prev_ = data_ - 1;
        }

        Heap(DataType_ *data, const int size, const bool copy = false)
            : prev_(data - 1), data_(data), capacity_(size), size_(size), deleteData(copy) {
            for (int i = size >> 1; i; --i)
                percolateDown(i, size);
        }

        ~Heap() {
            if (deleteData)
                delete[] data_;
        }

        void insert(const DataType_ &data) {
            if (size_ == capacity_)
                throw std::runtime_error("heap is full");

            int i, child;
            for (i = ++size_; i && data < prev_[child = i >> 1]; i = child)
                prev_[i] = prev_[child];

            prev_[i] = data;
        }

        void pop() {
            if (size_ == 0)
                throw std::runtime_error("heap is empty");
            *data_ = prev_[size_];
            percolateDown(1, --size_);
        }

        DataType_ &top() {
            if (size_ == 0)
                throw std::runtime_error("heap is empty");
            return *data_;
        }

    private:
        void percolateDown(const int top, const int last) {
            const DataType_ topValue = prev_[top];

            int father = top;
            for (int child; (child = father << 1) <= last; father = child) {
                if (child != last && prev_[child + 1] < prev_[child])
                    child++;
                if (prev_[child] < topValue)
                    prev_[father] = prev_[child];
                else
                    break;
            }
            prev_[father] = topValue;
        }

        DataType_ *prev_;
        DataType_ *data_;
        int capacity_;
        int size_ = 0;
        bool deleteData;
    };
}

#endif //HEAP_H
