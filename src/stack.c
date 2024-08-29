#include "stack.h"
#include "error.h"

void saw_stack_init(saw_stack_t *stack)
{
    stack->top = -1;
}

void saw_stack_push(saw_stack_t *stack, saw_stack_element_t element)
{
    if (stack->top == SAW_STACK_MAX_HEIGHT - 1)
        SAW_ERROR("Stack overflow");

    stack->top += 1;
    stack->arr[stack->top] = element;
}

saw_stack_element_t saw_stack_pop(saw_stack_t *stack)
{
    if (stack->top == -1)
        SAW_ERROR("Stack underflow");

    stack->top -= 1;
    return stack->arr[stack->top + 1];
}

void saw_stack_free(saw_stack_t *stack) {}