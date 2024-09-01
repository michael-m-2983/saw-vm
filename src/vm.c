#include "vm.h"
#include "opcode.h"
#include "error.h"

void saw_vm_init(vm_t *vm, FILE *fp)
{
    vm->running = 1;
    vm->fp = fp;
    saw_stack_init(&vm->stack);
}

void saw_insn_push(vm_t *vm)
{
    saw_byte_t byte;

    if (fread(&byte, sizeof(saw_byte_t), 1, vm->fp) != sizeof(saw_byte_t))
        SAW_ERROR("Failed to read a byte!");

    saw_stack_push(&vm->stack, byte);
}

void saw_insn_dup(vm_t *vm)
{
    saw_byte_t byte = saw_stack_pop(&vm->stack);

    saw_stack_push(&vm->stack, byte);
    saw_stack_push(&vm->stack, byte);
}

void saw_insn_add(vm_t *vm)
{
    saw_byte_t b = saw_stack_pop(&vm->stack);
    saw_byte_t a = saw_stack_pop(&vm->stack);

    saw_stack_push(&vm->stack, a + b);
}

void saw_insn_sub(vm_t *vm)
{
    saw_byte_t b = saw_stack_pop(&vm->stack);
    saw_byte_t a = saw_stack_pop(&vm->stack);

    saw_stack_push(&vm->stack, a - b);
}

void saw_insn_multiply(vm_t *vm)
{
    saw_byte_t b = saw_stack_pop(&vm->stack);
    saw_byte_t a = saw_stack_pop(&vm->stack);

    saw_stack_push(&vm->stack, a * b);
}

void saw_insn_stackdump(vm_t *vm)
{
    puts("#======== Stack Dump ========#");
    printf("#\tStack Top:\t%2d   #\n#\t\t\t     #\n", vm->stack.top);

    for (int i = 0; i < vm->stack.top + 1; i++)
    {
        printf("#\tStack[%d] =\t%2d   #\n", i, vm->stack.arr[i]);
    }

    puts("#============================#");
}

void saw_vm_step(vm_t *vm)
{
    opcode_t opcode;

    if (fread(&opcode, sizeof(opcode_t), 1, vm->fp) != sizeof(opcode))
        SAW_ERROR("Failed to read next opcode! This binary may be missing a %s instruction!", OPCODE_NAMES[OP_HALT]);

    if (opcode < OPCODE_MIN || opcode > OPCODE_MAX)
        SAW_ERROR("Invalid opcode '0x%X'!", opcode);

    switch (opcode)
    {
    case OP_NOP:
        break;
    case OP_PUSH_BYTE:
        saw_insn_push(vm);
        break;
    case OP_POP:
        saw_stack_pop(&vm->stack);
        break;
    case OP_DUP:
        saw_insn_dup(vm);
        break;
    case OP_ADD:
        saw_insn_add(vm);
        break;
    case OP_SUB:
        saw_insn_sub(vm);
        break;
    case OP_MULTIPLY:
        saw_insn_multiply(vm);
        break;
    case OP_STACKDUMP:
        saw_insn_stackdump(vm);
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