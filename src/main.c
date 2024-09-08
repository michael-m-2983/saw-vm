#include "vm.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "error.h"
#include "types.h"
#include "debugger.h"

int main(int argc, char **argv)
{
    FILE *fp;
    vm_t vm;

    if (argc < 2)
        SAW_ERROR("Usage: %s bytecode.saw", argv[0]);

#ifdef SAW_DEBUG_MODE
    if (strncmp(argv[1], "-", 1) == 0)
        SAW_ERROR("You cannot pass input from stdin while using the debugger. "
                  "Recompile without SAW_DEBUG_MODE defined.");
#endif

    fp = strncmp(argv[1], "-", 1) ? fopen(argv[1], "rb") : stdin;

    if (fp == NULL)
        SAW_ERROR("Error while opening bytecode file: %s", strerror(errno));

    saw_vm_init(&vm, fp);

#ifdef SAW_DEBUG_MODE
    saw_debugger(&vm);
#else
    while (vm.running)
        saw_vm_step(&vm);
#endif

    if (fp != stdin)
        fclose(fp);

    saw_vm_free(&vm);

    return 0;
}
