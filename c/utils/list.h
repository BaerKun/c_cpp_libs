#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "node.h"

typedef struct List_ List, *ListPtr;

struct List_ {
    NodePtr head;
    NodePtr tail;
};

static inline ListPtr newList() {
    const ListPtr list = malloc(sizeof(List));
    list->head = list->tail = NULL;
    return list;
}

static void listClear(const ListPtr list) {
    nodeClear(&list->head);
    list->tail = NULL;
}

static NodePtr *listFind(const ListPtr list, LIST_ELEMENT_TYPE const element) {
    return nodeFind(&list->head, element);
}

static void listPush(const ListPtr list, LIST_ELEMENT_TYPE const element) {
    nodeInsert(&list->head, element);
    if (list->tail == NULL)
        list->tail = list->head;
}

static LIST_ELEMENT_TYPE listPop(const ListPtr list) {
    if (list->head == NULL)
        return (LIST_ELEMENT_TYPE){0};

    const NodePtr node = nodeUnlink(&list->head);
    LIST_ELEMENT_TYPE const elem = node->element;
    free(node);
    if (list->head == NULL)
        list->tail = NULL;
    return elem;
}

static void listDelete(const ListPtr list, LIST_ELEMENT_TYPE const element) {
    const NodePtr node = NodeUnlinkWithData(&list->head, element);
    if (node == NULL)
        return;

    free(node);
    if (list->head == NULL)
        list->tail = NULL;
}

static inline void listEnqueue(const ListPtr list, LIST_ELEMENT_TYPE const element) {
    nodeInsert(&list->tail, element);
    if (list->tail == NULL)
        list->head = list->tail;
}

static inline LIST_ELEMENT_TYPE listDequeue(const ListPtr list) {
    return listPop(list);
}

static inline void listDestroy(const ListPtr list) {
    nodeClear(&list->head);
    free(list);
}

#endif //LINKEDLIST_H
