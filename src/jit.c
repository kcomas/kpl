
#include "jit.h"

extern inline jit *jit_i(void);

extern inline void jit_a(jit *const j, uint8_t b);

extern inline void jit_f(jit *j);

extern inline fn_stk *fn_stk_i(size_t size);

extern inline void fn_stk_a(fn_stk **stk, const code *const c);

void fn_stk_b(fn_stk **stk, const code *const c) {
    for (size_t i = 0; i < c->len; i++)
        if (c->ops[i].oc == OP_C(PV) && c->ops[i].ot == TYPE(FN)) fn_stk_b(stk, c->ops[i].od.c);
    fn_stk_a(stk, c);
}

extern inline void fn_stk_f(fn_stk *f);
