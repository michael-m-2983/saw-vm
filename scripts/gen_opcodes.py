import os

opcodes = [name.replace(".c", "") for name in os.listdir("src/opcodes/")]

opcode_defines = []
for index, opcode in enumerate(opcodes):
    opcode_defines.append(f"#define OPCODE_{opcode.upper()} {index}")
opcode_defines = "\n".join(opcode_defines)

opcode_functions = []
for opcode in opcodes:
    opcode_functions.append(open(f"src/opcodes/{opcode}.c", "r").read())
opcode_functions = "\n\n".join(opcode_functions)

opcode_function_pointer_assignments = []
for opcode in opcodes:
    opcode_function_pointer_assignments.append(f"opcode_functions[OPCODE_{opcode.upper()}] = &opcode_{opcode};")
opcode_function_pointer_assignments = "\n".join(opcode_function_pointer_assignments)

HEADER_CODE = f"""\
#ifndef OPCODE_H
#define OPCODE_H

#include "stack.h"

void (*opcode_functions[{len(opcodes)}])(stack_t *stack);

{opcode_defines}

#endif
"""

SOURCE_CODE = f"""\
#include "opcode.h"

{opcode_functions}

void init_opcodes() {{
{opcode_function_pointer_assignments}
}}
"""

with open("src/opcode.h", "w") as fp:
    fp.write(HEADER_CODE)
with open("src/opcode.c", "w") as fp:
    fp.write(SOURCE_CODE)