#ifndef STACK_H
#define STACK_H

#include <stdexcept>

namespace cpp_libs {
    template<typename DataType_>
    class Stack {
    public:
        explicit Stack(const int capacity) : data_(new DataType_[capacity]), capacity_(capacity) {
        }

        ~Stack() {
            delete[] data_;
        }

        [[nodiscard]] bool isEmpty() const {
            return top_ == 0;
        }

        void push(const DataType_ &data) {
            if (top_ == capacity_) {
                throw std::runtime_error("Stack is full");
            }
            data_[top_++] = data;
        }

        void pop() {
            if (top_ == 0) {
                throw std::runtime_error("Stack is empty");
            }
            --top_;
        }

        DataType_ &top() {
            if (top_ == 0) {
                throw std::runtime_error("Stack is empty");
            }
            return data_[top_ - 1];
        }

        void clear() {
            top_ = 0;
        }

    private:
        DataType_ *data_;
        int capacity_;
        int top_ = 0;
    };
}

#endif //STACK_H
