#ifndef STACK_H
#define STACK_H

#ifndef SAW_STACK_MAX_HEIGHT
#define SAW_STACK_MAX_HEIGHT 50
#endif

#include "types.h"

typedef saw_byte_t saw_stack_element_t;

typedef struct
{
    saw_stack_element_t arr[SAW_STACK_MAX_HEIGHT];
    int top; // The top of the stack- this is -1 by default.
} saw_stack_t;

/**
 * Initialize a stack
 * 
 * @param stack A pointer to the stack
 */
void saw_stack_init(saw_stack_t *stack);

/**
 * Push a value to the stack
 *
 * @param stack A pointer to the stack
 * @param element The element to push
 */
void saw_stack_push(saw_stack_t *stack, saw_stack_element_t element);

/**
 * Pop a value off of the stack.
 *
 * @param stack A pointer to the stack
 * @return The value
 */
saw_stack_element_t saw_stack_pop(saw_stack_t *stack);

/**
 * A function to free the contents of the stack. 
 * 
 * At the moment, this function does nothing. 
 * 
 * Be sure to call it anyway though, as it might be needed in the future.
 * 
 * @param stack A pointer to the stack
 */
void saw_stack_free(saw_stack_t *stack);

#endif