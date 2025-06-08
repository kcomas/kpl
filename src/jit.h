
#pragma once

#include "kpl.h"
#include "code.h"

typedef struct _jit {
    int len, size; // compat with getpagesize
    uint8_t *a; // address
} jit;

inline jit *jit_i(void) {
    jit *j = calloc(1, sizeof(jit));
    j->size = getpagesize();
    j->a = mmap(NULL, j->size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    return j;
}

inline void jit_f(jit *j) {
    munmap(j->a, j->size);
    free(j);
}
