#ifndef STACK_H
#define STACK_H

#include <stdint.h>

#define MAX_STACK_HEIGHT 50

typedef uint16_t stack_element;

typedef struct {
    uint8_t top;
    stack_element data[MAX_STACK_HEIGHT];
} stack_t;

void stack_push(stack_t *stack, stack_element element);
stack_element stack_pop(stack_t *stack);

#endif