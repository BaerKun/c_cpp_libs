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
    stack->top = -1;
    stack->capacity = capacity;
}

static inline void stackPush(const StackPtr stack, STACK_DATA_TYPE const data) {
    stack->data[++stack->top] = data;
}

static inline void stackPop(const StackPtr stack, STACK_DATA_TYPE *const ptr) {
    if(ptr)
        *ptr = stack->data[stack->top];
    --stack->top;
}

static inline STACK_DATA_TYPE *stackTop(const Stack *const stack) {
    return stack->data + stack->top;
}

static inline int stackIsEmpty(const Stack *const stack) {
    return stack->top == -1;
}

static inline void stackFreeData(const StackPtr stack) {
    free(stack->data);
}

#endif //STACK_H
