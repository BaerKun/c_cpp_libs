#include "private/stack.h"
#include <stdlib.h>

void graphStackInit(GraphStack *const stack, const GraphSize capacity) {
  stack->size = 0;
  stack->data = malloc(capacity * sizeof(GraphId));
}

void graphStackRelease(const GraphStack *const stack) {
  free(stack->data);
}