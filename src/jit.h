
#pragma once

#include "kpl.h"
#include "code.h"

#define JIT_STAT(N) JIT_STAT_##N

typedef enum {
    JIT_STAT(OK),
    JIT_STAT(SG_T_INV), // inv type for sg
    JIT_STAT(LG_T_INV), // inv type for lg
    JIT_STAT(LM_INV),
    JIT_STAT(PV_T_INV), // no push for type
    JIT_STAT(INV_VR_IDX_T),
    JIT_STAT(ZOO_T_INV),
    JIT_STAT(ADD_T_INV),
    JIT_STAT(SUB_T_INV),
    JIT_STAT(MUL_T_INV),
    JIT_STAT(DIV_T_INV),
    JIT_STAT(EQ_T_INV),
    JIT_STAT(GT_T_INV),
    JIT_STAT(LT_T_INV),
    JIT_STAT(OR_T_INV),
    JIT_STAT(CSTSG_T_INV),
    JIT_STAT(CNCTSG_T_INV),
    JIT_STAT(WFD_T_INV),
    JIT_STAT(RCI_T_INV),
    JIT_STAT(RCD_T_INV),
    JIT_STAT(RCF_T_INV),
    JIT_STAT(GC_T_INV),
    JIT_STAT(GCTSV_T_INV),
    JIT_STAT(GCVR_T_INV),
    JIT_STAT(DEL_T_INV),
    JIT_STAT(IF_ELSE_INV),
    JIT_STAT(INV_CODE),
} jit_stat;

const char *jit_stat_str(jit_stat jstat);

#ifndef FN_STK_SIZE
    #define FN_STK_SIZE 10
#endif

typedef struct {
    size_t len, size, nops;
    code *fn[];
} fn_stk;

inline fn_stk *fn_stk_i(al *const a, size_t size) {
    fn_stk *stk = ala(a, sizeof(fn_stk) + sizeof(code*) * size);
    stk->size = size;
    return stk;
}

#ifndef FN_STK_RSIZE
    #define FN_STK_RSIZE 2
#endif

inline void fn_stk_a(al *const a, fn_stk **stk, code *const c) {
    if ((*stk)->len == (*stk)->size) {
        fn_stk *ns = fn_stk_i(a, (*stk)->size * FN_STK_RSIZE);
        for (size_t i = 0; i < (*stk)->len; i++) ns->fn[ns->len++] = (*stk)->fn[i];
        alf(*stk);
        *stk = ns;
    }
    (*stk)->fn[(*stk)->len++] = c;
}

void fn_stk_b(al *const a, fn_stk **stk, code *const c); // create stack from code scan

inline void fn_stk_f(fn_stk *f) {
    alf(f);
}

#define JIT_FLG(N) JIT_FLG_##N

typedef enum {
    JIT_FLG(TD) = (1 << 0) // thread code
} jit_flgs;

#ifndef BYTES_PER_OP
    #define BYTES_PER_OP 50
#endif

void jit_i(al *const a, size_t nops, jit **j, uint8_t flgs);

inline jit_stat jit_er(mod *const m, const char *const fnn, jit_stat jstat, const op *const o) {
    if (jstat == JIT_STAT(OK)) return jstat;
    er_itm *ei = er_itm_i(m->r->a, ER(JIT), fnn, jit_stat_str(jstat));
    if (o && o->a) {
        ei->lno = o->a->t.lno;
        ei->cno = o->a->t.cno;
    }
    er_a(m->r->e, ei);
    return jstat;
}

inline void jit_a(jit *j, uint8_t b) {
    j->h[j->len++] = b;
}

inline void jit_b(jit *j, size_t len, ...) {
    va_list args;
    va_start(args, len);
    for (size_t i = 0; i < len; i++) jit_a(j, (uint8_t) va_arg(args, int));
    va_end(args);
}

inline void jit_c(jit *j, size_t len, uint8_t b[]) {
    for (size_t i = 0; i < len; i++) jit_a(j, b[i]);
}

jit_stat jit_code(mod *const m, code *const c, jit_fn *const jf, jit *j, bool done); // jf is the self fn entry

jit_stat jit_stk(mod *const m, fn_stk *const stk, jit *j, bool done);
