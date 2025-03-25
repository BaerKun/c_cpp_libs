#ifndef LINK_LIKE_H
#define LINK_LIKE_H

#include "link_node.h"

typedef struct LinkLike_ LinkLike;

struct LinkLike_ {
    LinkNodePtr head;
    LinkNodePtr tail;
};

static inline void linkLikeInit(LinkLike *link) {
    link->head = link->tail = NULL;
}

static void listClear(LinkLike *link) {
    nodeClear(&link->head);
    link->tail = NULL;
}

static void listPush(LinkLike * link, LINK_NODE_DATA_TYPE const data) {
    nodeInsert(&link->head, data);
    if (link->tail == NULL)
        link->tail = link->head;
}

static LINK_NODE_DATA_TYPE listPop(LinkLike * link) {
    if (link->head == NULL)
        return (LINK_NODE_DATA_TYPE){0};

    const LinkNodePtr node = nodeUnlink(&link->head);
    LINK_NODE_DATA_TYPE const data = node->data;
    free(node);
    if (link->head == NULL)
        link->tail = NULL;
    return data;
}

static void listDelete(LinkLike * link, LINK_NODE_DATA_TYPE const data) {
    const LinkNodePtr node = NodeUnlinkWithData(&link->head, data);
    if (node == NULL)
        return;

    free(node);
    if (link->head == NULL)
        link->tail = NULL;
}

static inline void listEnqueue(LinkLike * link, LINK_NODE_DATA_TYPE const data) {
    nodeInsert(&link->tail, data);
    if (link->tail == NULL)
        link->head = NULL;
}

static inline LINK_NODE_DATA_TYPE listDequeue(LinkLike * link) {
    return listPop(link);
}

static inline void listDestroy(LinkLike * link) {
    nodeClear(&link->head);
}

#endif //LINK_LIKE_H
