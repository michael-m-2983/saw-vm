#include <stdio.h>
#include <stdlib.h>

#define SAW_ERROR(...)                \
    do                                \
    {                                 \
        fprintf(stderr, __VA_ARGS__); \
        puts("");                     \
        exit(1);                      \
    } while (0)
