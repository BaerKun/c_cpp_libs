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
            return top_ == -1;
        }

        void push(const DataType_ &data) {
            if (top_ + 1 == capacity_) {
                throw std::runtime_error("Stack is full");
            }
            data_[++top_] = data;
        }

        void pop() {
            if (isEmpty()) {
                throw std::runtime_error("Stack is empty");
            }
            --top_;
        }

        DataType_ &top() {
            if (isEmpty()) {
                throw std::runtime_error("Stack is empty");
            }
            return data_[top_];
        }

        void clear() {
            top_ = -1;
        }

    private:
        DataType_ *data_;
        int top_ = -1;
        int capacity_;
    };
}

#endif //STACK_H
