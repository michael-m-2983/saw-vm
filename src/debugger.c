#ifdef SAW_DEBUG_MODE
#include "debugger.h"

#include <string.h>
#include <stdio.h>

#include "error.h"
#include "opcode.h"

void saw_debugger(vm_t *vm)
{
    char command[20];
    int debugger_running = 1;

    while (debugger_running)
    {
        fprintf(stdout, ">>> ");
        fflush(stdout);
        scanf("%19s", command);

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
            else if (c == 'b') // Execute all instructions until next breakpoint
            {
                saw_short_t initial_breakpoint_count = vm->breakpoint_count;
                while (vm->running && vm->breakpoint_count == initial_breakpoint_count)
                {
                    saw_vm_step(vm);
                }
            }
            else if (c == 'q') // Exit
            {
                vm->running = 0;
                debugger_running = 0;
                break;
            }
            else if (c == 's') // Stack dump
            {
                saw_insn_stackdump(vm);
            }
            else if (c == 'o') // Print offset
            {
                const long offset = ftell(vm->fp);
                fprintf(stdout, "[Offset]: %ld / 0x%lX\n", offset, offset);
            }
            else if (c == 'd') // Create vm dump
            {
                FILE *fp = fopen("saw-vm.dump", "wb");
                fwrite(vm, sizeof(vm_t), 1, fp);
                fclose(fp);
            } else {
                printf("Unsupported debugger instruction '%c'!\n", c);
            }
        }
    }
}

#endif