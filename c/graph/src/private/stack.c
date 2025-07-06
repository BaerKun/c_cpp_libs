#include "private/stack.h"
#include <stdlib.h>

GraphStack *graphStackInit(const GraphSize capacity) {
  GraphStack *stack = malloc(sizeof(GraphStack) + capacity * sizeof(GraphId));
  stack->size = 0;
  return stack;
}

void graphStackRelease(GraphStack *const stack) {
  free(stack);
}