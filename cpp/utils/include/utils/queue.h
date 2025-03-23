#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept>

namespace cpp_libs{
    template <typename DataType_>
    class Queue{
    public:
        explicit Queue(const int capacity) : data_(new DataType_[capacity]), capacity_(capacity){
        }

        ~Queue(){
            delete[] data_;
        }

        [[nodiscard]] bool isEmpty() const {
            return size_ == 0;
        }

        void enqueue(const DataType_ &data){
            if (size_ == capacity_)
                throw std::runtime_error("Queue is full");
            ++size_;
            data_[rear_] = data;
            if (++rear_ == capacity_)
                rear_ = 0;
        }

        void dequeue() {
            if (size_ == 0)
                throw std::runtime_error("Queue is empty");
            --size_;
            if (++front_ == capacity_)
                front_ = 0;
        }

        DataType_ &front(){
            if (size_ == 0)
                throw std::runtime_error("Queue is empty");
            return data_[front_];
        }

        void clear() {
            front_ = rear_ = 0;
        }
    private:
        DataType_ *data_;
        int capacity_;
        int front_ = 0;
        int rear_ = 0;
        int size_ = 0;
    };
}

#endif //QUEUE_H
