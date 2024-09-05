#include "vm.h"
#include "opcode.h"
#include "error.h"
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

    switch (opcode)
    {
    case OP_NOP:
        break;
    case OP_PUSH_BYTE:
        saw_insn_push_byte(vm);
        break;
    case OP_POP:
        saw_stack_pop(&vm->stack);
        break;
    case OP_DUP:
        saw_insn_dup(vm);
        break;
    case OP_DUP_DOUBLE:
        saw_insn_dup_double(vm);
        break;
    case OP_SWAP:
        saw_insn_swap(vm);
        break;
    case OP_SWAPDOWN:
        saw_insn_swapdown(vm);
        break;
    case OP_ADD:
        saw_insn_add(vm);
        break;
    case OP_SUBTRACT:
        saw_insn_subtract(vm);
        break;
    case OP_MULTIPLY:
        saw_insn_multiply(vm);
        break;
    case OP_DIVIDE:
        saw_insn_divide(vm);
        break;
    case OP_NEGATE:
        saw_insn_negate(vm);
        break;
    case OP_INCREMENT:
        saw_insn_increment(vm);
        break;
    case OP_BITWISE_NOT:
        saw_insn_bitwise_not(vm);
        break;
    case OP_BITWISE_AND:
        saw_insn_bitwise_and(vm);
        break;
    case OP_BITWISE_OR:
        saw_insn_bitwise_or(vm);
        break;
    case OP_BITWISE_XOR:
        saw_insn_bitwise_xor(vm);
        break;
    case OP_SQRT:
        saw_insn_sqrt(vm);
        break;
    case OP_ABS:
        saw_insn_abs(vm);
        break;
    case OP_STACKDUMP:
        saw_insn_stackdump(vm);
        break;
    case OP_STACKPRINT:
        saw_insn_stackprint(vm);
        break;
    case OP_BRANCH:
        saw_insn_branch(vm);
        break;
    case OP_BRANCH_IF_ZERO:
        saw_insn_branch_if_zero(vm);
        break;
    case OP_PUSH_IP:
        saw_insn_push_ip(vm);
        break;
    case OP_BREAKPOINT:
#ifdef SAW_DEBUG_MODE
        vm->breakpoint_count += 1;
#endif
        break;
    case OP_HALT:
        vm->running = 0;
        break;
    default:
        SAW_ERROR("Unimplemented opcode '%s'!", OPCODE_NAMES[opcode]);
        break;
    }
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
    saw_stack_push(&vm->stack, abs(saw_stack_pop(&vm->stack)));
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

void saw_insn_push_ip(vm_t *vm)
{
    long ip = ftell(vm->fp) - 1;    // Gets the current ip
    saw_stack_push(&vm->stack, ip); // We want to push the offset of the next instruction.
}
