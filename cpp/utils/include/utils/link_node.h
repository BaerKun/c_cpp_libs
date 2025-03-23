#ifndef LINK_NODE_H
#define LINK_NODE_H

namespace cpp_libs {
    template<typename DataType_>
    struct LinkNode {
        LinkNode *next = nullptr;
        DataType_ data;

        LinkNode() = default;

        explicit LinkNode(const DataType_ &data) : data(data) {
        }

        static void insert(LinkNode **prevNextPtr, LinkNode *node) {
            node->next = *prevNextPtr;
            *prevNextPtr = node;
        }

        static void insert(LinkNode **prevNextPtr, const DataType_ &data) {
            insert(prevNextPtr, new LinkNode(data));
        }

        static LinkNode *unlink(LinkNode **prevNextPtr) {
            LinkNode *node = *prevNextPtr;
            *prevNextPtr = node->next;
            return node;
        }

        static void remove(LinkNode **prevNextPtr) {
            delete unlink(prevNextPtr);
        }

        static LinkNode **find(LinkNode **headNextPtr, const DataType_ &data) {
            LinkNode **prev = headNextPtr;
            for (LinkNode *node = *prev; node && node->data != data; prev = &node->next, node = *prev) {
            }
            return prev;
        }

        static void clear(LinkNode **headNextPtr) {
            for (const LinkNode *node = *headNextPtr, *next; node; node = next) {
                next = node->next;
                delete node;
            }
            *headNextPtr = nullptr;
        }

        ~LinkNode() = default;
    };
}

#endif //LINK_NODE_H
