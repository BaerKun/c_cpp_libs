#ifndef ID_MANAGER_H
#define ID_MANAGER_H

#include "graph/type.h"

typedef struct {
  GraphSize range;
  GraphId freeHead;
  GraphId *iterHead;
  GraphId *next;
} IdManager;

struct GraphManager_ {
  GraphId *iterHead;
  IdManager edge, vert;
};

#endif // ID_MANAGER_H
