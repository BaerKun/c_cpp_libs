#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdio.h>

#ifndef STACK_ELEMENT_TYPE
#define STACK_ELEMENT_TYPE int
#endif

typedef struct Stack_ Stack, *StackPtr;

struct Stack_{
    STACK_ELEMENT_TYPE *elements;
    int capacity;
    int top;
};

static inline StackPtr newStack(const int capacity){
    const StackPtr stack = (StackPtr)malloc(sizeof(Stack));

    stack->capacity = capacity;
    stack->top = 0;
    stack->elements = (STACK_ELEMENT_TYPE *)malloc(sizeof(STACK_ELEMENT_TYPE) * capacity);

    return stack;
}

static inline void stackPush(const StackPtr stack, STACK_ELEMENT_TYPE const element){
    stack->elements[stack->top++] = element;
}

static inline STACK_ELEMENT_TYPE stackPop(const StackPtr stack){
    return stack->elements[--stack->top];
}

static inline STACK_ELEMENT_TYPE stackPeek(const StackPtr stack){
    return stack->elements[stack->top - 1];
}

static inline void stackDestroy(const StackPtr stack){
    free(stack->elements);
    free(stack);
}

#endif //STACK_H
