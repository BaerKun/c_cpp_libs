#ifndef LINK_LIKE_H
#define LINK_LIKE_H

#include "link_node.h"

typedef struct LinkLike_ LinkLike;

struct LinkLike_ {
    LinkNodePtr head;
    LinkNodePtr *tail;
};

static inline void linkLikeInit(LinkLike *const link) {
    link->head = NULL;
    link->tail = &link->head;
}

static inline void linkClear(LinkLike *const link) {
    nodeClear(&link->head);
    link->tail = &link->head;
}

static inline void linkPush(LinkLike *const link, LINK_NODE_DATA_TYPE const data) {
    nodeInsert(&link->head, data);
    if (link->tail == &link->head)
        link->tail = &link->head->next;
}

static inline void linkPop(LinkLike *const link) {
    nodeDelete(&link->head);
    if (link->head == NULL)
        link->tail = &link->head;
}

static inline LINK_NODE_DATA_TYPE *linkTop(const LinkLike *const link) {
    return &link->head->data;
}

static void linkFilter(LinkLike *const link, LINK_NODE_DATA_TYPE const data) {
    const LinkNodePtr node = NodeUnlinkWithData(&link->head, data);
    if (node == NULL)
        return;

    free(node);
    if (link->head == NULL)
        link->tail = &link->head;
}

static inline void linkEnqueue(LinkLike *const link, LINK_NODE_DATA_TYPE const data) {
    const LinkNodePtr node = newNode(data);
    *link->tail = node;
    link->tail = &node->next;
}

static inline void linkDequeue(LinkLike *const link) {
    linkPop(link);
}

static inline int linkEmpty(const LinkLike *const link) {
    return link->head == NULL;
}

#endif //LINK_LIKE_H
