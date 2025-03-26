#ifndef LINK_LIKE_H
#define LINK_LIKE_H

#include "link_node.h"

typedef struct LinkLike_ LinkLike;

struct LinkLike_ {
    LinkNodePtr head;
    LinkNodePtr *tail;
};

static inline void linkLikeInit(LinkLike *link) {
    link->head = NULL;
    link->tail = &link->head;
}

static inline void linkClear(LinkLike *link) {
    nodeClear(&link->head);
    link->tail = &link->head;
}

static inline void linkPush(LinkLike *link, LINK_NODE_DATA_TYPE const data) {
    nodeInsert(&link->head, data);
    if (link->tail == &link->head)
        link->tail = &link->head->next;
}

static inline void linkPop(LinkLike *link) {
    nodeDelete(&link->head);
    if (link->head == NULL)
        link->tail = &link->head;
}

static inline LINK_NODE_DATA_TYPE linkTop(const LinkLike *link) {
    return link->head->data;
}

static void linkFilter(LinkLike *link, LINK_NODE_DATA_TYPE const data) {
    const LinkNodePtr node = NodeUnlinkWithData(&link->head, data);
    if (node == NULL)
        return;

    free(node);
    if (link->head == NULL)
        link->tail = &link->head;
}

static inline void linkEnqueue(LinkLike *link, LINK_NODE_DATA_TYPE const data) {
    const LinkNodePtr node = newNode(data);
    *link->tail = node;
    link->tail = &node->next;
}

static inline LINK_NODE_DATA_TYPE linkDequeue(LinkLike *link) {
    LINK_NODE_DATA_TYPE const data = linkTop(link);
    linkPop(link);
    return data;
}

#endif //LINK_LIKE_H
