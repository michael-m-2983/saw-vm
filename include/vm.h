#ifndef VM_H
#define VM_H

#include <stdio.h>
#include "stack.h"

typedef struct {
    int running;
    saw_stack_t stack;
    FILE *fp;
} vm_t;

/**
 * Initializes a new vm.
 * 
 * @param vm The vm to initialize.
 * @param fp The bytecode file to read from.
 */
void saw_vm_init(vm_t *vm, FILE *fp);

/**
 * Executes a single bytecode instruction.
 * 
 * @param vm The vm.
 */
void saw_vm_step(vm_t *vm);

/**
 * Frees a vm_t.
 * 
 * @param vm The vm.
 */
void saw_vm_free(vm_t *vm);

#endif