#ifndef LINK_NODE_H
#define LINK_NODE_H

#ifndef LINK_NODE_ELEM_TYPE
#define LINK_NODE_ELEM_TYPE int
#endif

#include <stdlib.h>

typedef LINK_NODE_ELEM_TYPE LinkNodeElemType_;
typedef struct LinkNode_ LinkNode, *LinkNodePtr;

struct LinkNode_ {
  LinkNodePtr next;
  LinkNodeElemType_ data;
};

static inline LinkNodePtr newNode(LinkNodeElemType_ const data) {
  const LinkNodePtr node = malloc(sizeof(LinkNode));
  node->data = data;
  node->next = NULL;
  return node;
}

static inline void nodeInsert(LinkNodePtr *const prevNextPtr,
                              LinkNodeElemType_ const data) {
  const LinkNodePtr node = malloc(sizeof(LinkNode));
  node->data = data;
  node->next = *prevNextPtr;
  *prevNextPtr = node;
}

static inline void nodePushBack(const LinkNodePtr node,
                                LinkNodeElemType_ const data) {
  nodeInsert(&node->next, data);
}

static inline LinkNodePtr nodeUnlink(LinkNodePtr *const prevNextPtr) {
  const LinkNodePtr node = *prevNextPtr;
  *prevNextPtr = node->next;
  return node;
}

static LinkNodePtr *nodeFind(LinkNodePtr *const headNextPtr,
                             LinkNodeElemType_ const data) {
  LinkNodePtr *prev = headNextPtr;
  for (LinkNodePtr node = *prev; node && node->data != data;
       prev = &node->next, node = *prev) {
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

#endif // LINK_NODE_H
