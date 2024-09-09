#include "vm.h"
#include "opcode.h"
#include "error.h"
#include "stack.h"
#include <math.h>

void saw_vm_init(vm_t *vm, FILE *fp)
{
    vm->running = 1;
    vm->fp = fp;
    saw_stack_init(&vm->stack);
}

void saw_vm_step(vm_t *vm)
{
    opcode_t opcode;
#ifdef SAW_DEBUG_MODE
    long offset = ftell(vm->fp);
#endif

    if (fread(&opcode, sizeof(opcode_t), 1, vm->fp) != sizeof(opcode))
        SAW_ERROR("Failed to read next opcode! This binary may be missing a %s instruction!", OPCODE_NAMES[OP_HALT]);

    if (opcode < OPCODE_MIN || opcode > OPCODE_MAX)
        SAW_ERROR("Invalid opcode '0x%X'!", opcode);

#ifdef SAW_DEBUG_MODE
    fprintf(stderr, "[saw-vm]: 0x%lX %s stack=%d\n", offset, OPCODE_NAMES[opcode], vm->stack.top + 1);
#endif

    saw_insn(vm, opcode);
}

void saw_vm_free(vm_t *vm)
{
    saw_stack_free(&vm->stack);
}

void saw_insn_push_byte(vm_t *vm)
{
    saw_byte_t byte;

    if (fread(&byte, sizeof(saw_byte_t), 1, vm->fp) != sizeof(saw_byte_t))
        SAW_ERROR("Failed to read a byte!");

    saw_stack_push(&vm->stack, (saw_stack_element_t)byte);
}

void saw_insn_nop(vm_t *vm)
{
    // Do nothing
}

void saw_insn_pop(vm_t *vm)
{
    saw_stack_pop(&vm->stack);
}

void saw_insn_breakpoint(vm_t *vm)
{
#ifdef SAW_DEBUG_MODE
    vm->breakpoint_count += 1;
#endif
}

void saw_insn_dup(vm_t *vm)
{
    saw_long_t byte = saw_stack_pop(&vm->stack);

    saw_stack_push(&vm->stack, byte);
    saw_stack_push(&vm->stack, byte);
}

void saw_insn_dup_double(vm_t *vm)
{
    saw_stack_element_t b = saw_stack_pop(&vm->stack);
    saw_stack_element_t a = saw_stack_pop(&vm->stack);
    saw_stack_push(&vm->stack, a);
    saw_stack_push(&vm->stack, b);
    saw_stack_push(&vm->stack, a);
    saw_stack_push(&vm->stack, b);
}

void saw_insn_swap(vm_t *vm)
{
    saw_long_t b = saw_stack_pop(&vm->stack);
    saw_long_t a = saw_stack_pop(&vm->stack);

    saw_stack_push(&vm->stack, b);
    saw_stack_push(&vm->stack, a);
}

void saw_insn_swapdown(vm_t *vm)
{
    saw_byte_t n; // The swapdown argument / how far down to swap the top with

    if (fread(&n, sizeof(saw_byte_t), 1, vm->fp) != sizeof(saw_byte_t))
        SAW_ERROR("Failed to read swapdown byte!");

    if(n < 0) SAW_ERROR("Cannot execute swapdown instruction: Stack underreach! Cannot swapdown negative by negative amounts. (n=%d, stack_top=%d)", n, vm->stack.top);
    if(n > vm->stack.top) SAW_ERROR("Cannot execute swapdown instruction: Stack overreach! (n=%d, stack_top=%d)", n, vm->stack.top);

    saw_stack_element_t tmp = vm->stack.arr[vm->stack.top];
    vm->stack.arr[vm->stack.top] = vm->stack.arr[vm->stack.top - n];
    vm->stack.arr[vm->stack.top - n] = tmp;
}

void saw_insn_add(vm_t *vm)
{
    saw_long_t b = saw_stack_pop(&vm->stack);
    saw_long_t a = saw_stack_pop(&vm->stack);

    saw_stack_push(&vm->stack, a + b);
}

void saw_insn_subtract(vm_t *vm)
{
    saw_long_t b = saw_stack_pop(&vm->stack);
    saw_long_t a = saw_stack_pop(&vm->stack);

    saw_stack_push(&vm->stack, a - b);
}

void saw_insn_multiply(vm_t *vm)
{
    saw_long_t b = saw_stack_pop(&vm->stack);
    saw_long_t a = saw_stack_pop(&vm->stack);

    saw_stack_push(&vm->stack, a * b);
}

void saw_insn_divide(vm_t *vm)
{
    saw_long_t b = saw_stack_pop(&vm->stack);
    saw_long_t a = saw_stack_pop(&vm->stack);

    saw_stack_push(&vm->stack, a / b);
}

void saw_insn_negate(vm_t *vm)
{
    saw_stack_push(&vm->stack, -saw_stack_pop(&vm->stack));
}

void saw_insn_increment(vm_t *vm)
{
    saw_byte_t amt;

    if (fread(&amt, sizeof(saw_byte_t), 1, vm->fp) != sizeof(saw_byte_t))
        SAW_ERROR("Failed to read the increment amount byte!");

    saw_stack_element_t x = saw_stack_pop(&vm->stack);

    saw_stack_push(&vm->stack, (saw_stack_element_t)x + amt);
}

void saw_insn_bitwise_not(vm_t *vm)
{
    saw_stack_push(&vm->stack, -(saw_stack_pop(&vm->stack) + 1));
}

void saw_insn_bitwise_and(vm_t *vm)
{
    saw_long_t b = saw_stack_pop(&vm->stack);
    saw_long_t a = saw_stack_pop(&vm->stack);

    saw_stack_push(&vm->stack, a & b);
}

void saw_insn_bitwise_or(vm_t *vm)
{
    saw_long_t b = saw_stack_pop(&vm->stack);
    saw_long_t a = saw_stack_pop(&vm->stack);

    saw_stack_push(&vm->stack, a | b);
}

void saw_insn_bitwise_xor(vm_t *vm)
{
    saw_long_t b = saw_stack_pop(&vm->stack);
    saw_long_t a = saw_stack_pop(&vm->stack);

    saw_stack_push(&vm->stack, a ^ b);
}

void saw_insn_sqrt(vm_t *vm)
{
    saw_stack_push(&vm->stack, sqrtl(saw_stack_pop(&vm->stack)));
}

void saw_insn_abs(vm_t *vm)
{
    saw_stack_push(&vm->stack, labs(saw_stack_pop(&vm->stack)));
}

void saw_insn_stackdump(vm_t *vm)
{
    puts("#======== Stack Dump ========#");
    printf("#\tStack Top:\t%2d   #\n#\t\t\t     #\n", vm->stack.top);

    for (int i = 0; i < vm->stack.top + 1; i++)
    {
        printf("#\tStack[%d] =\t%2ld   #\n", i, vm->stack.arr[i]);
    }

    puts("#============================#");
}

void saw_insn_stackprint(vm_t *vm)
{
    printf("%ld\n", vm->stack.arr[vm->stack.top]);
}

void saw_insn_branch(vm_t *vm)
{
    saw_long_t offset = saw_stack_pop(&vm->stack);
    fseek(vm->fp, offset, 0);
}

void saw_insn_branch_if_zero(vm_t *vm)
{
    saw_stack_element_t conditional = saw_stack_pop(&vm->stack);
    saw_long_t offset = saw_stack_pop(&vm->stack);

    if (conditional == 0)
        fseek(vm->fp, offset, 0);
}

void saw_insn_branch_if_positive(vm_t *vm)
{
    saw_stack_element_t number = saw_stack_pop(&vm->stack);

    saw_long_t offset = saw_stack_pop(&vm->stack);

    if (number > 0)
        fseek(vm->fp, offset, 0);
}

void saw_insn_branch_if_equal(vm_t *vm)
{
    saw_stack_element_t b = saw_stack_pop(&vm->stack);
    saw_stack_element_t a = saw_stack_pop(&vm->stack);

    saw_long_t offset = saw_stack_pop(&vm->stack);

    if (a == b)
        fseek(vm->fp, offset, 0);
}

void saw_insn_push_ip(vm_t *vm)
{
    long ip = ftell(vm->fp) - 1;    // Gets the current instruction pointer
    saw_stack_push(&vm->stack, ip); // We want to push the offset of the next instruction.
}

void saw_insn_create_array(vm_t *vm) {
    saw_byte_t size;

    if(fread(&size, sizeof(size), 1, vm->fp) != sizeof(size))
        SAW_ERROR("Failed to read size when creating array!");

    //TODO: validate the size more, or perhaps do this in another way.

    saw_stack_push(&vm->stack, (saw_stack_element_t) calloc(sizeof(saw_stack_element_t), size));
}

void saw_insn_update_array(vm_t *vm) {
    saw_stack_element_t data, index, array_ptr;

    data = saw_stack_pop(&vm->stack);
    index = saw_stack_pop(&vm->stack);
    array_ptr = saw_stack_pop(&vm->stack);

    ((saw_stack_element_t *) array_ptr)[index] = data;
    saw_stack_push(&vm->stack, array_ptr);
}

//TODO: an instruction for getting array contents

void saw_insn_destroy_array(vm_t *vm) {
    saw_stack_element_t array = saw_stack_pop(&vm->stack);
    free((saw_stack_element_t *) array);
}

/**
 * Reads a basic string from a file.
 * 
 * TODO: This needs to be improved, but works for now.
 * security, read it all at once instead of char by char, support utf, etc.
 *
 * Caller must free the returned pointer using free().
 *
 * @param fp The file pointer to read from.
 * @return A string. Must be free()'d.
 */
char *read_str(FILE *fp) {
    saw_byte_t length;

    if(fread(&length, sizeof(length), 1, fp) != sizeof(length))
        SAW_ERROR("Failed to read string length.");

    if(length < 0) SAW_ERROR("Invalid string length.");

    char *result = calloc(sizeof(char), length);

    char c;
    for(int i = 0; i < length; i++) {
        if (fread(&c, sizeof(c), 1, fp) != sizeof(c))
            SAW_ERROR("Failed to read character in string!");

        result[i] = c;
    }

    return result;
}

void saw_insn_call(vm_t *vm) {
    saw_byte_t call_type;
    if(fread(&call_type, sizeof(call_type), 1, vm->fp) != sizeof(call_type)) SAW_ERROR("Failed to read call type!");

    char *fn_name = read_str(vm->fp);
    char *fn_desc = read_str(vm->fp);

    //TODO: call the function.
    printf("[saw-vm]: Calling function with call_type=%d, fn=%s, desc=%s\n", call_type, fn_name, fn_desc);
}

void saw_insn_halt(vm_t *vm) {
    vm->running = 0;
}

void saw_insn_unknown(vm_t *vm, opcode_t opcode) {
    SAW_ERROR("Unknown opcode '0x%X'! (decimal=%d)", opcode, opcode);
}