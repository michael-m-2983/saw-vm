#include <stdio.h>
#include "error.h"
#include "object_file.h"

/**
 * TODO List:
 * 
 * - Make the freads fail if nothing is read
 * - Improve error handling system
 * - Start interpreting bytecode
 * - Add more opcodes
 * - Add opcode arguments
 */

int main(int argc, char **argv) {

    if(argc != 2) {
        panic("Invalid arguments! Usage: saw <code.saw>"); //TODO: print full usage
    }

    char *file_name = argv[1];

    object_file obj = read_object_file(file_name);

    return 0;
}