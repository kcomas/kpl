
#pragma once

#include "kpl.h"
#include "mod.h"
#include "code.h"

#define JIT_STAT(N) JIT_STAT_##N

typedef enum {
    JIT_STAT(OK),
    JIT_STAT(SG_T_INV), // inv type for sg
    JIT_STAT(LG_T_INV), // inv type for lg
    JIT_STAT(PV_T_INV), // no push for type
    JIT_STAT(ZOO_T_INV),
    JIT_STAT(ADD_T_INV),
    JIT_STAT(SUB_T_INV),
    JIT_STAT(GT_T_INV),
    JIT_STAT(LT_T_INV),
    JIT_STAT(CSTSG_T_INV),
    JIT_STAT(CNCTSG_T_INV),
    JIT_STAT(WFD_T_INV),
    JIT_STAT(GC_T_INV),
    JIT_STAT(INV_CODE)
} jit_stat;

#ifndef FN_STK_SIZE
    #define FN_STK_SIZE 10
#endif

typedef struct {
    size_t len, size;
    code *fn[];
} fn_stk;

inline fn_stk *fn_stk_i(size_t size) {
    fn_stk *stk = calloc(1, sizeof(fn_stk) + sizeof(code*) * size);
    stk->size = size;
    return stk;
}

#ifndef FN_STK_RSIZE
    #define FN_STK_RSIZE 2
#endif

inline void fn_stk_a(fn_stk **stk, code *const c) {
    if ((*stk)->len == (*stk)->size) {
        fn_stk *ns = fn_stk_i((*stk)->size * FN_STK_RSIZE);
        for (size_t i = 0; i < (*stk)->len; i++) ns->fn[ns->len++] = (*stk)->fn[i];
        free(*stk);
        *stk = ns;
    }
    (*stk)->fn[(*stk)->len++] = c;
}

void fn_stk_b(fn_stk **stk, code *const c); // create stack from code scan

inline void fn_stk_f(fn_stk *f) {
    free(f);
}

typedef struct _jit {
    int len, np, size; // len, num pages, total size, int for compat with getpagesize
    uint8_t *a; // address
} jit;

#ifndef JIT_NUM_PAGE
    #define JIT_NUM_PAGE 1
#endif

inline jit *jit_i(int np) {
    jit *j = calloc(1, sizeof(jit));
    j->np = np;
    j->size = getpagesize() * np;
    j->a = mmap(NULL, j->size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    return j;
}

#ifndef JIT_NUM_PAGE_RSIZE
    #define JIT_NUM_PAGE_RSIZE 2
#endif

inline void jit_f(jit *j) {
    munmap(j->a, j->size);
    free(j);
}

inline void jit_a(jit **j, uint8_t b) {
    if ((*j)->len == (*j)->size) {
        jit *nj = jit_i((*j)->np * JIT_NUM_PAGE_RSIZE);
        nj->len = (*j)->len;
        memcpy(nj->a, (*j)->a, nj->len);
        jit_f(*j);
        *j = nj;
    }
    (*j)->a[(*j)->len++] = b;
}

inline void jit_b(jit **j, size_t len, ...) {
    va_list args;
    va_start(args, len);
    for (size_t i = 0; i < len; i++) jit_a(j, (uint8_t) va_arg(args, int));
    va_end(args);
}

inline void jit_c(jit **j, size_t len, uint8_t *b) {
    for (size_t i = 0; i < len; i++) jit_a(j, b[i]);
}

jit_stat jit_code(mod *const m, code *const c, jit **j);

jit_stat jit_stk(mod *const m, fn_stk *const stk, jit **j);
