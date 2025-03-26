#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

#ifndef STACK_DATA_TYPE
#define STACK_DATA_TYPE int
#endif

typedef struct Stack_ Stack, *StackPtr;

struct Stack_ {
    STACK_DATA_TYPE *data;
    int top;
    int capacity;
};

static inline void stackInit(const StackPtr stack, const int capacity) {
    stack->data = (STACK_DATA_TYPE *) malloc(sizeof(STACK_DATA_TYPE) * capacity);
    stack->top = 0;
    stack->capacity = capacity;
}

static inline void stackPush(const StackPtr stack, STACK_DATA_TYPE const element) {
    stack->data[stack->top++] = element;
}

static inline void stackPop(const StackPtr stack) {
    --stack->top;
}

static inline STACK_DATA_TYPE stackTop(const StackPtr stack) {
    return stack->data[stack->top - 1];
}

static inline void stackFreeData(const StackPtr stack) {
    free(stack->data);
}

#endif //STACK_H
