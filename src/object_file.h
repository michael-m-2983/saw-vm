#ifndef OBJECT_FILE_H
#define OBJECT_FILE_H

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;

typedef struct {
    u8 format_version_major;
    u8 format_version_minor;
} object_file_metadata;

typedef struct {
    u8 opcode;
} instruction;

typedef struct {
    // magic number - b"saw" - not stored in this struct
    object_file_metadata metadata;

    u16 instruction_count;
    u16 *instructions;
} object_file;

object_file read_object_file(char *file_name);

#endif