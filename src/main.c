#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "opcode.h"
#include "error.h"
#include "types.h"
#include "stack.h"

int main(int argc, char **argv)
{

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s bytecode.saw\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *inputFile = argv[1];

    FILE *fp = fopen(inputFile, "rb");

    if (fp == NULL)
    {
        SAW_ERROR("Error while opening bytecode file: %s", strerror(errno));
    }

    int running = 1;
    saw_stack_t stack;

    saw_stack_init(&stack);

    while (running)
    {
        opcode_t opcode;
        size_t s = fread(&opcode, sizeof(opcode_t), 1, fp);

        if (s != sizeof(opcode))
        {
            SAW_ERROR("Failed to read next opcode! This binary may be missing a %s instruction!", OPCODE_NAMES[OP_HALT]);
        }

        if (opcode < OPCODE_MIN || opcode > OPCODE_MAX)
        {
            SAW_ERROR("Invalid opcode '0x%X'!", opcode);
        }

        saw_byte_t byte;

        switch (opcode)
        {
        case OP_PUSH_BYTE:

            if (fread(&byte, sizeof(saw_byte_t), 1, fp) != sizeof(saw_byte_t))
                SAW_ERROR("Failed to read a byte!");

            saw_stack_push(&stack, byte);

            printf("Pushing a byte to the stack.\n\t--> %d\n", byte);
            break;
        case OP_POP:
            byte = saw_stack_pop(&stack);
            printf("Popping a byte from the stack.\n\t<--- %d\n", byte);
            break;
	case OP_DUP:
	    byte = saw_stack_pop(&stack);
	    printf("Duplicated byte on stack\n\t<--> %d\n", byte);
	    saw_stack_push(&stack, byte);
	    saw_stack_push(&stack, byte);
	    break;
        case OP_STACKDUMP:
            puts("#======== Stack Dump ========#");
            printf("#\tStack Top:\t%2d   #\n#\t\t\t     #\n", stack.top);

            for (int i = 0; i < stack.top + 1; i++)
            {
                printf("#\tStack[%d] =\t%2d   #\n", i, stack.arr[i]);
            }

            puts("#============================#");
            break;
        case OP_HALT:
            running = 0;
            break;
        default:
            SAW_ERROR("Unimplemented opcode '%s'!", OPCODE_NAMES[opcode]);
            break;
        }
    }

    fclose(fp);

    return 0;
}
