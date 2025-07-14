#include "private/disjoint_set.h"
#include <stdlib.h>
#include <string.h>

GraphDisjointSet *graphDisjointCreate(const GraphSize size) {
  GraphDisjointSet *set = malloc(size * sizeof(GraphId));
  memset(set, -1, size * sizeof(GraphId));
  return set;
}

void graphDisjointRelease(GraphDisjointSet *set) { free(set); }