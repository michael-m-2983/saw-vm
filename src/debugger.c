#include "debugger.h"

#include <string.h>
#include <stdio.h>

#include "error.h"
#include "opcode.h"

void saw_debugger(vm_t *vm)
{
    char command[20];

    while (1)
    {
        fprintf(stdout, ">>> ");
        fflush(stdout);
        scanf("%20s", command);

        const size_t command_length = strlen(command);
        if (command_length > 20)
            SAW_ERROR("Buffer overflow in debug command string!");

        for (int i = 0; i < command_length; i++)
        {
            const char c = command[i];

            if (c == 'n') // Step forward by 1
            {
                saw_vm_step(vm);
            }
            else if (c == 'N') // Execute as normal
            {
                while (vm->running)
                    saw_vm_step(vm);
            }
            else if (c == 'q') // Exit
            {
                vm->running = 0;
                break;
            }
            else if (c == 's') // Stack dump
            {
                saw_insn_stackdump(vm);
            }
        }
    }
}