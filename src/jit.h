
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
    #define FN_STK_SIZE 10
#endif

typedef struct {
    size_t len, size;
    const code *fn[];
} fn_stk;

inline fn_stk *fn_stk_i(size_t size) {
    fn_stk *stk = calloc(1, sizeof(fn_stk) + sizeof(code*) * size);
    stk->size = size;
    return stk;
}

#ifndef FN_STK_RSIZE
    #define FN_STK_RSIZE 2
#endif

inline void fn_stk_a(fn_stk **stk, const code *const c) {
    if ((*stk)->len == (*stk)->size) {
        fn_stk *ns = fn_stk_i((*stk)->size * FN_STK_RSIZE);
        for (size_t i = 0; i < (*stk)->len; i++) ns->fn[ns->len++] = (*stk)->fn[i];
        free(*stk);
        *stk = ns;
    }
    (*stk)->fn[(*stk)->len++] = c;
}

void fn_stk_b(fn_stk **stk, const code *const c); // create stack from code scan

inline void fn_stk_f(fn_stk *f) {
    free(f);
}
