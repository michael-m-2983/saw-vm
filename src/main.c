#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "opcode.h"
#include "error.h"

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

        switch (opcode)
        {
        case OP_HALT:
            running = 0;
            printf("Program exit!\n");
            break;
        default:
            printf("Opcode: %s\n", OPCODE_NAMES[opcode]);
            break;
        }
    }

    fclose(fp);

    return 0;
}