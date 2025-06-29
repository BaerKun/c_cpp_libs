#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdint.h>

#ifndef STACK_ELEM_TYPE
#define STACK_ELEM_TYPE int
#endif

typedef STACK_ELEM_TYPE StackElemType_;
typedef struct Stack_ Stack;

struct Stack_ {
  StackElemType_ *data;
  uint64_t top;
};

static inline void stackInit(Stack *const stack, const uint64_t capacity) {
  stack->data = (StackElemType_ *)malloc(sizeof(StackElemType_) * capacity);
  stack->top = 0;
}

static inline void stackPush(Stack *const stack, StackElemType_ const data) {
  stack->data[stack->top++] = data;
}

static inline void stackPop(Stack *const stack) { --stack->top; }

static inline StackElemType_ *stackTop(const Stack *const stack) {
  return stack->data + stack->top - 1;
}

static inline int stackEmpty(const Stack *const stack) {
  return stack->top == 0;
}

static inline void stackFreeData(const Stack *const stack) {
  free(stack->data);
}

#endif // STACK_H
