#include "object_file.h"

#include "error.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void check_magic_number(FILE *fp) {
    char magic_number[4];

    fread(&magic_number, 3, 1, fp);
    magic_number[3] = '\0';

    if(strncmp(magic_number, "saw", 3)) {
        panic("Invalid magic number!");
    }
}

object_file_metadata read_metadata(FILE *fp) {
    object_file_metadata metadata = {};


    fread(&metadata.format_version_major, sizeof(u8), 1, fp);
    fread(&metadata.format_version_minor, sizeof(u8), 1, fp);

    return metadata;
}

object_file read_object_file(char *file_name) {
    FILE *fp = fopen(file_name, "rb");

    check_magic_number(fp);

    object_file_metadata metadata = read_metadata(fp);

    printf("Saw v%d.%d\n", metadata.format_version_major, metadata.format_version_minor);

    u16 insn_count = 0;
    fread(&insn_count, sizeof(u16), 1, fp);

    u16 *instructions = calloc(insn_count, sizeof(u16));

    fread(instructions, sizeof(u16), insn_count, fp);

    //TODO: store instructions in a better format

    free(instructions); //TODO: better memory management, i.e. create and free functions for object files

    object_file obj = {};

    return obj;
}