#ifndef ID_MANAGER_H
#define ID_MANAGER_H

#include "graph/type.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
  GraphId max;
  GraphId freeHead;
  GraphId iterHead;
  GraphId *next;
} VertManager;
typedef struct {
  GraphId max;
  GraphId freeHead;
  GraphId *iterHead;
  GraphId *next;
} EdgeManager;

struct GraphIdManager_ {
  VertManager vert;
  EdgeManager edge;
};

static GraphIdManager *idManagerCreate(const GraphSize vertCap,
                                       const GraphSize edgeCap) {
  GraphIdManager *const manager = malloc(sizeof(GraphIdManager));
  VertManager *const vert = &manager->vert;
  vert->max = vert->freeHead = 0;
  vert->iterHead = -1;
  vert->next = malloc(vertCap * sizeof(GraphId));
  for (GraphSize i = 1; i < vertCap; --i) vert->next[i - 1] = i;
  vert->next[vertCap - 1] = -1;

  EdgeManager *const edge = &manager->edge;
  edge->max = edge->freeHead = 0;
  edge->iterHead = malloc(vertCap * sizeof(GraphId));
  memset(edge->iterHead, -1, vertCap * sizeof(GraphId));
  edge->next = malloc(2 * edgeCap * sizeof(GraphId));
  for (GraphSize i = 1; i < edgeCap; ++i) edge->next[i - 1] = i;
  edge->next[edgeCap - 1] = -1;
  return manager;
}

static GraphId vertGetNew(VertManager *vert) {
  const GraphId id = vert->freeHead;
  vert->freeHead = vert->next[id];
  vert->next[id] = vert->iterHead;
  vert->iterHead = id;
  if (vert->freeHead > vert->max) vert->max = vert->freeHead;
  return id;
}

static GraphId edgeGetNew(EdgeManager *edge, const GraphId from) {
  const GraphId id = edge->freeHead;
  edge->freeHead = edge->next[id];
  edge->next[id] = edge->iterHead[from];
  edge->iterHead[from] = id;
  if (edge->freeHead > edge->max) edge->max = edge->freeHead;
  return id;
}

#endif // ID_MANAGER_H
