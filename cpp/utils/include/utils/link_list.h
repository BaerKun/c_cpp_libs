#ifndef LINK_LIST_H
#define LINK_LIST_H

#include <stdexcept>
#include "link_node.h"

namespace cpp_libs {
    template<typename DataType_>
    class LinkList {
    public:
        LinkList() {
            tail_ = &head_;
        }

        ~LinkList() {
            Node::clear(&head_);
        }

        [[nodiscard]] bool isEmpty() const {
            return head_ == nullptr;
        }

        LinkNode<DataType_> *getLinkNodes() {
            return head_;
        }

        void push(const DataType_ &data) {
            Node::insert(&head_, data);
            if (&head_ == tail_)
                tail_ = &head_->next;
        }

        void pop() {
            if (head_ == nullptr)
                throw std::runtime_error("empty list");
            Node::remove(&head_);
            if (head_ == nullptr)
                tail_ = &head_;
        }

        DataType_ &front() {
            if (head_ == nullptr)
                throw std::runtime_error("empty list");
            return head_->data;
        }

        void enqueue(const DataType_ &data) {
            Node::insert(tail_, data);
            tail_ = &(*tail_)->next;
        }

        void dequeue() {
            pop();
        }

        void clear() {
            Node::clear(&head_);
            tail_ = &head_;
        }
    private:
        using Node = LinkNode<DataType_>;

        Node *head_ = nullptr;
        Node **tail_;
    };
}

#endif //LINK_LIST_H
