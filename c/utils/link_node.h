#ifndef LINK_NODE_H
#define LINK_NODE_H

#include <stdlib.h>

#ifndef LINK_NODE_DATA_TYPE
#define LINK_NODE_DATA_TYPE int
#endif

typedef struct LinkNode_ LinkNode, *LinkNodePtr;

struct LinkNode_ {
    LinkNodePtr next;
    LINK_NODE_DATA_TYPE data;
};

static inline LinkNodePtr newNode(LINK_NODE_DATA_TYPE const data) {
    const LinkNodePtr node = malloc(sizeof(LinkNode));
    node->data = data;
    node->next = NULL;
    return node;
}

static inline void nodeInsert(LinkNodePtr *const prevNextPtr, LINK_NODE_DATA_TYPE const data) {
    const LinkNodePtr node = malloc(sizeof(LinkNode));
    node->data = data;
    node->next = *prevNextPtr;
    *prevNextPtr = node;
}

static inline void nodePushBack(const LinkNodePtr node, LINK_NODE_DATA_TYPE const data) {
    nodeInsert(&node->next, data);
}

static inline LinkNodePtr nodeUnlink(LinkNodePtr *const prevNextPtr) {
    const LinkNodePtr node = *prevNextPtr;
    *prevNextPtr = node->next;
    return node;
}

static LinkNodePtr *nodeFind(LinkNodePtr *const headNextPtr, LINK_NODE_DATA_TYPE const data) {
    LinkNodePtr *prev = headNextPtr;
    for (LinkNodePtr node = *prev; node && node->data != data; prev = &node->next, node = *prev) {
    }
    return prev;
}

static inline void nodeDelete(LinkNodePtr *const prevNextPtr) {
    free(nodeUnlink(prevNextPtr));
}

static void nodeClear(LinkNodePtr *const headNextPtr) {
    for (LinkNodePtr node = *headNextPtr, next; node; node = next) {
        next = node->next;
        free(node);
    }
    *headNextPtr = NULL;
}

#endif //LINK_NODE_H
