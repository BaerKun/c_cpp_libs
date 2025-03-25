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

static inline LinkNodePtr newNode(LINK_NODE_DATA_TYPE const element) {
    const LinkNodePtr node = malloc(sizeof(LinkNode));
    node->data = element;
    node->next = NULL;
    return node;
}

static inline void nodeInsert(LinkNodePtr *prevNextPtr, LINK_NODE_DATA_TYPE const element) {
    const LinkNodePtr node = malloc(sizeof(LinkNode));
    node->data = element;
    node->next = *prevNextPtr;
    *prevNextPtr = node;
}

static inline LinkNodePtr nodeUnlink(LinkNodePtr *prevNextPtr) {
    const LinkNodePtr node = *prevNextPtr;
    *prevNextPtr = node->next;
    return node;
}

static LinkNode **nodeFind(LinkNodePtr *headNextPtr, LINK_NODE_DATA_TYPE const element) {
    LinkNodePtr *prev = headNextPtr;
    for (LinkNodePtr node = *prev; node && node->data != element; prev = &node->next, node = *prev) {
    }
    return prev;
}

static LinkNodePtr NodeUnlinkWithData(LinkNodePtr *headNextPtr, LINK_NODE_DATA_TYPE const element) {
    LinkNodePtr *prev = nodeFind(headNextPtr, element);
    return *prev ? nodeUnlink(prev) : NULL;
}

static inline void nodeDelete(LinkNodePtr *prevNextPtr) {
    LinkNode *node = nodeUnlink(prevNextPtr);
    free(node);
}

static void nodeClear(LinkNodePtr *headNextPtr) {
    for (LinkNodePtr node = *headNextPtr; node; node = *headNextPtr) {
        *headNextPtr = node->next;
        free(node);
    }
}

#endif //LINK_NODE_H
