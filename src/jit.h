
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

inline void jit_a(jit *const j, uint8_t b) {
    j->a[j->len++] = b;
}

inline void jit_f(jit *j) {
    munmap(j->a, j->size);
    free(j);
}

#ifndef FN_STK_SIZE
    #define FN_STK_SIZE 20
#endif

typedef struct {
    size_t len, size;
    const code *fn[FN_STK_SIZE];
} fn_stk;

inline void fn_stk_i(fn_stk *const stk) {
    stk->len = 0;
    stk->size = FN_STK_SIZE;
    memset(stk->fn, 0, sizeof(code*) * FN_STK_SIZE);
}

inline void fn_stk_a(fn_stk *const stk, const code *const c) {
    stk->fn[stk->len++] = c;
}

void fn_stk_b(fn_stk *const stk, const code *const c); // create stack from code scan
