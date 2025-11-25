// % {}

void opcode_dup(stack_t *stack) {
    stack_element e = stack_pop(stack);
    stack_push(stack, e);
    stack_push(stack, e);
}