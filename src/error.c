#include "error.h"
#include <stdio.h>
#include <stdlib.h>

void panic(char *msg) {
    printf("\e[1mError\e[22m: %s\n", msg);

    exit(1);
}