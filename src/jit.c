
#include "jit.h"

extern inline fn_stk *fn_stk_i(size_t size);

extern inline void fn_stk_a(fn_stk **stk, code *const c);

void fn_stk_b(fn_stk **stk, code *const c) {
    for (size_t i = 0; i < c->len; i++)
        if (c->ops[i].oc == OP_C(PV) && c->ops[i].ot == TYPE(FN)) fn_stk_b(stk, c->ops[i].od.c);
    fn_stk_a(stk, c);
}

extern inline void fn_stk_f(fn_stk *f);

extern inline jit *jit_i(int np);

extern inline void jit_f(jit *j);

extern inline void jit_a(jit **j, uint8_t b);

extern inline void jit_b(jit **j, size_t len, ...);

extern inline void jit_c(jit **j, size_t len, uint8_t *b);

static void op_set_jidx(const jit *const j, op *const o) {
    o->jidx = j->len;
}

static void op_set_jlen(const jit *const j, op *const o) {
    o->jlen = j->len - o->jidx;
}

static void mov_reg(jit **j, bool rexwr, uint8_t reg, uint8_t *buf) {
    jit_a(j, rexwr ? 0x4C : 0x48);
    jit_a(j, 0xB8 + reg);
    jit_c(j, sizeof(void*), buf);
}

#define SET_BUF(BUF, DATA, SIZE) memset(BUF, 0, sizeof(void*)); \
    memcpy(BUF, DATA, SIZE)

jit_stat jit_code(mod *const m, code *const c, jit **j) {
    jit_stat jstat;
    op *o;
    void *fp;
    uint8_t buf[sizeof(void*)];
    for (size_t i = 0;  i < c->len; i++) {
        o = &c->ops[i];
        switch (o->oc) {
            case OP_C(EFN):
                op_set_jidx(*j, o);
                jit_b(j, 4, 0x55, 0x48, 0x89, 0xE5); // push rbp, mov rbp rsp
                op_set_jlen(*j, o);
                break;
            case OP_C(RFN):
                op_set_jidx(*j, o);
                jit_b(j, 2, 0x5D, 0xC3); // pop rbp, ret
                op_set_jlen(*j, o);
                break;
            // TODO
            case OP_C(CFN):
                op_set_jidx(*j, o);
                jit_b(j, 3, 0x58, 0xFF, 0xD0); // pop rax, call rax
                op_set_jlen(*j, o);
                break;
            case OP_C(AG):
                op_set_jidx(*j, o);
                SET_BUF(buf, &m, sizeof(mod*));
                mov_reg(j, false, 0x07, buf); // mov rdi m
                SET_BUF(buf, &o->od.u3, sizeof(uint8_t));
                mov_reg(j, false, 0x06, buf); // mov rsi &o->od.u3
                fp = (void*) &mod_ag;
                SET_BUF(buf, fp, sizeof(void*));
                mov_reg(j, false, 0x00, buf); // mov rag mod_ag
                jit_b(j, 2, 0xFF, 0xD0); // call rax
                op_set_jlen(*j, o);
                break;
            case OP_C(SG):
                op_set_jidx(*j, o);
                // TODO
                op_set_jlen(*j, o);
                break;
            case OP_C(LG):
                op_set_jidx(*j, o);
                // TODO
                op_set_jlen(*j, o);
                break;
            // TODO
            case OP_C(PV):
                op_set_jidx(*j, o);
                // TODO
                op_set_jlen(*j, o);
                break;
            default:
                return JIT_STAT(INV_CODE);
        }
    }
    return JIT_STAT(OK);
}

jit_stat jit_stk(mod *const m, fn_stk *const stk, jit **j) {
    jit_stat jstat;
    for (size_t i = 0; i < stk->len; i++) {
        stk->fn[i]->jf = (jit_fn*) &(*j)->a[(*j)->len];
        if ((jstat = jit_code(m, stk->fn[i], j)) != JIT_STAT(OK)) return jstat;
    }
    return JIT_STAT(OK);
}
