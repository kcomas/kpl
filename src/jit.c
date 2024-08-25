
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
    memcpy(BUF, &DATA, SIZE)

#define SET_REG(TGT, SIZEOF, REXRW, REGN) SET_BUF(buf, TGT, sizeof(SIZEOF)); \
    mov_reg(j, REXRW, 0x0##REGN, buf)

#define SET_FP(FN) fp = (void*) &FN

#define CT_SET_FN(T, FN) case TYPE(T): \
    SET_FP(FN); \
    break

#define SET_REG_CALL(REXRW, REGN) SET_REG(fp, void*, REXRW, REGN); \
    jit_b(j, 2, 0xFF, 0xD##REGN);

#define OPPVT(T, D, CT) case TYPE(T): \
    SET_BUF(buf, o->od.D, sizeof(CT)); \
    mov_reg(j, true, 0x01, buf); \
    jit_b(j, 2, 0x4C, 0x51); \
    op_set_jlen(*j, o); \
    break

// TODO float and double xmm regs
#define C_OP_C_BOP(N, FN) case OP_C(N): \
    op_set_jidx(*j, o); \
    jit_a(j, 0x5E); \
    jit_a(j, 0x5F); \
    switch (o->od.t) { \
        CT_SET_FN(I6, var_##FN##_i6); \
        default: \
            return JIT_STAT(N##_T_INV); \
    } \
    SET_REG_CALL(false, 0); \
    jit_a(j, 0x50); \
    op_set_jlen(*j, o); \
    break

#ifndef IF_STK_LEN
    #define IF_STK_LEN 10
#endif

static jit_stat jit_if(mod *const m, code *const c, jit **j) {
    jit_stat jstat;
    op *o;
    // int for compat with getpagesize
    int jmpp, jmpl;
    int stki = 0;
    int stk[IF_STK_LEN];
    memset(stk, 0, sizeof(int) * IF_STK_LEN);
    for (size_t i = 0; i < c->len; i++) {
        o = &c->ops[i];
        op_set_jidx(*j, o);
        if ((jstat = jit_code(m, o->od.of->cond, j)) != JIT_STAT(OK)) return jstat; // 0 or 1 on stack
        jit_b(j, 2, 0x41, 0x5B); // pop r11
        jit_b(j, 3, 0x4D, 0x31, 0xE4); // xor r12 12
        jit_b(j, 3, 0x4D, 0x39, 0xE3); // cmp r11 r12
        jit_b(j, 2, 0x0F, 0x84); // je 0
        jmpp = (*j)->len;
        jit_b(j, 4, 0x00, 0x00, 0x00, 0x00); // filled after body
        if ((jstat = jit_code(m, o->od.of->body, j)) != JIT_STAT(OK)) return jstat;
        jit_a(j, 0xE9); // jmp
        stk[stki++] = (*j)->len;
        jit_b(j, 4, 0x00, 0x00, 0x00, 0x00); // filled after if
        jmpl = (*j)->len - jmpp - sizeof(int);
        memcpy((*j)->a + jmpp, &jmpl, sizeof(int)); // fill cond skip
        op_set_jlen(*j, o);
    }
    while (stki > 0) {
        jmpl = (*j)->len - stk[stki - 1] - sizeof(int);
        memcpy((*j)->a + stk[stki - 1], &jmpl, sizeof(int)); // fill body skip
        stki--;
    }
    return JIT_STAT(OK);
}

jit_stat jit_code(mod *const m, code *const c, jit **j) {
    jit_stat jstat;
    op *o;
    void *fp;
    uint32_t asp; // arg stack pos
    static uint8_t buf[sizeof(void*)];
    for (size_t i = 0;  i < c->len; i++) {
        o = &c->ops[i];
        switch (o->oc) {
            case OP_C(EFN):
                op_set_jidx(*j, o);
                jit_b(j, 4, 0x55, 0x48, 0x89, 0xE5); // push rbp, mov rbp rsp
                op_set_jlen(*j, o);
                break;
            case OP_C(PUSH):
                op_set_jidx(*j, o);
                jit_a(j, 0x50 + o->od.u3);
                op_set_jlen(*j, o);
                break;
            case OP_C(RFN):
                op_set_jidx(*j, o);
                if (o->od.t != TYPE(VD)) jit_a(j, 0x58); // pop rax TODO xmm
                //jit_b(j, 2, 0xC9, 0xC3); // leave, ret
                jit_b(j, 2, 0x5D, 0xC3); // leave, ret
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
                SET_REG(m, mod*, false, 7);
                SET_REG(o->od.u3, uint8_t, false, 6);
                SET_FP(mod_ag);
                SET_REG_CALL(false, 0);
                op_set_jlen(*j, o);
                break;
            case OP_C(SG):
                op_set_jidx(*j, o);
                SET_REG(m, mod*, false, 7);
                SET_REG(o->od.v.id, uint8_t, false, 6);
                jit_a(j, 0x5A); // pop rdx
                switch (o->od.v.t) {
                    CT_SET_FN(STR, mod_sg_var_sg);
                    CT_SET_FN(I6, mod_sg_i6);
                    CT_SET_FN(SG, mod_sg_var_sg);
                    CT_SET_FN(FN, mod_sg_jf);
                    default:
                        return JIT_STAT(SG_T_INV);
                }
                SET_REG_CALL(false, 0);
                op_set_jlen(*j, o);
                break;
            case OP_C(LG):
                op_set_jidx(*j, o);
                SET_REG(m, mod*, false, 7);
                SET_REG(o->od.v.id, uint8_t, false, 6);
                switch (o->od.v.t) {
                    CT_SET_FN(STR, mod_lg_var_sg);
                    CT_SET_FN(I6, mod_lg_i6);
                    CT_SET_FN(SG, mod_lg_var_sg);
                    CT_SET_FN(FN, mod_lg_jf);
                    default:
                        return JIT_STAT(LG_T_INV);
                }
                SET_REG_CALL(false, 0);
                jit_a(j, 0x50); // push rax
                op_set_jlen(*j, o);
                break;
            // TODO
            case OP_C(LA):
                op_set_jidx(*j, o);
                asp = (o->od.v.id + 2) * sizeof(void*);
                jit_b(j, 7, 0x4C, 0x8B, 0x8D, asp, 0x00, 0x00, 0x00); // mov r9 rbp+asp
                jit_b(j, 2, 0x41, 0x51); // push r9
                op_set_jlen(*j, o);
                break;
            case OP_C(PV):
                op_set_jidx(*j, o);
                switch (o->ot) {
                    case TYPE(STR):
                        SET_REG(o->od.sg, char*, false, 7);
                        SET_FP(var_sg_i_str);
                        SET_REG_CALL(false, 0);
                        jit_a(j, 0x50); // push rax
                        op_set_jlen(*j, o);
                        break;
                    // TODO
                    OPPVT(U6, u6, uint64_t);
                    // TODO
                    OPPVT(I6, i6, int64_t);
                    // TODO
                    OPPVT(SG, sg, var_sg*);
                    OPPVT(FN, c->jf, jit_fn*);
                    OPPVT(FD, fd, int);
                    default:
                        return JIT_STAT(PV_T_INV);
                }
                break;
            // TODO
            case OP_C(IF):
                op_set_jidx(*j, o);
                if ((jstat = jit_if(m, o->od.c, j)) != JIT_STAT(OK)) return jstat;
                op_set_jlen(*j, o);
                break;
            case OP_C(COND): return JIT_STAT(INV_CODE); // INTERNAL IN IF
            case OP_C(ZOO):
                op_set_jidx(*j, o);
                jit_a(j, 0x5F); // pop rdi
                switch (o->od.t) {
                    CT_SET_FN(U6, var_zoo_u6);
                    CT_SET_FN(I6, var_zoo_i6);
                    default:
                        return JIT_STAT(ZOO_T_INV);
                }
                SET_REG_CALL(false, 0);
                jit_a(j, 0x50); // push rax
                op_set_jlen(*j, o);
                break;
            // TODO
            case OP_C(CSTSG):
                op_set_jidx(*j, o);
                jit_a(j, 0x5F); // pop rdi
                switch (o->od.t) {
                    CT_SET_FN(U6, var_u6_sg);
                    CT_SET_FN(I6, var_i6_sg);
                    default:
                        return JIT_STAT(CSTSG_T_INV);
                }
                SET_REG_CALL(false, 0);
                jit_a(j, 0x50); // push rax
                op_set_jlen(*j, o);
                break;
            C_OP_C_BOP(ADD, add);
            C_OP_C_BOP(SUB, sub);
            // TODO
            case OP_C(NOT):
                op_set_jidx(*j, o);
                jit_a(j, 0x5F); // pop rdi
                SET_FP(var_not);
                SET_REG_CALL(false, 0);
                jit_a(j, 0x50); // push rax
                op_set_jlen(*j, o);
                break;
            // TODO
            case OP_C(CNCTSG):
                op_set_jidx(*j, o);
                jit_b(j, 2, 0x41, 0x5A); // pop r10 tgt sg/te
                jit_b(j, 2, 0x41, 0x59); // pop r9 cnct sg
                jit_b(j, 3, 0x4C, 0x89, 0xCF); // mov rdi r9
                jit_b(j, 3, 0x4C, 0x89, 0xD6); // move rsi r10
                switch (o->od.t) {
                    case TYPE(STR):
                    case TYPE(SG):
                        SET_FP(var_sg_cnct_sg_sg);
                        break;
                    default:
                        return JIT_STAT(CNCTSG_T_INV);
                }
                jit_b(j, 2, 0x41, 0x51); // push r9
                jit_b(j, 2, 0x41, 0x52); // push r10
                SET_REG_CALL(false, 0);
                jit_b(j, 2, 0x41, 0x5A); // pop r10 tgt sg/te
                jit_b(j, 2, 0x41, 0x59); // pop r9 cnct sg
                jit_a(j, 0x50); // push rax
                jit_b(j, 2, 0x41, 0x51); // push r9
                jit_b(j, 2, 0x41, 0x52); // push r10
                op_set_jlen(*j, o);
                break;
            case OP_C(WFD):
                op_set_jidx(*j, o);
                switch (o->od.t) {
                    case TYPE(STR):
                    case TYPE(SG):
                        jit_b(j, 2, 0x41, 0x5A); // pop r10
                        jit_b(j, 3, 0x4C, 0x89, 0xD7); // mov rdi r10
                        SET_FP(var_sg_str);
                        SET_REG_CALL(false, 0);
                        jit_a(j, 0x50); // push rax
                        SET_FP(var_sg_len);
                        SET_REG_CALL(false, 0);
                        jit_b(j, 3, 0x48, 0x89, 0xC2); // mov rdx, rax
                        jit_a(j, 0x5E); // str in rsi
                        break;
                    default:
                        return JIT_STAT(WFD_T_INV);
                }
                jit_a(j, 0x5F); // fd in rdi
                switch (o->od.t) {
                    case TYPE(STR):
                    case TYPE(SG):
                        jit_b(j, 2, 0x41, 0x52); // push r10
                    default:
                        break;
                }
                SET_FP(write);
                SET_REG_CALL(false, 0);
                op_set_jlen(*j, o);
                break;
            case OP_C(GC):
                op_set_jidx(*j, o);
                jit_a(j, 0x5F); // pop rdi
                switch (o->od.t) {
                    case TYPE(INT):
                    case TYPE(FLT):
                    case TYPE(U3):
                    case TYPE(U4):
                    case TYPE(U5):
                    case TYPE(U6):
                    case TYPE(I3):
                    case TYPE(I4):
                    case TYPE(I5):
                    case TYPE(I6):
                        break;
                    case TYPE(STR):
                    case TYPE(SG):
                        SET_FP(var_sg_f);
                        SET_REG_CALL(false, 0);
                        break;
                    default:
                        return JIT_STAT(GC_T_INV);
                }
                op_set_jlen(*j, o);
                break;
            case OP_C(GCTEI):
                // TODO
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
