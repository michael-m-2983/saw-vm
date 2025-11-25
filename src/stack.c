#include "stack.h"
#include "error.h"



void stack_push(stack_t *stack, stack_element element) {
    if(stack->top + 1 > MAX_STACK_HEIGHT) {
        panic("Stack overflow!");
    }
    stack->data[++stack->top] = element;
}

stack_element stack_pop(stack_t *stack) {
    if(stack->top <= 0) {
        panic("Stack underflow!");
    }
    return stack->data[stack->top--];
}