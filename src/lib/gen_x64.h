
#pragma once

#include "vr.h"
#include "gen.h"
#include "as_x64.h"

#define GEN_OP(N) GEN_OP_##N

typedef enum {
    GEN_OP(_START),
    GEN_OP(LBL),
    GEN_OP(ENTER),
    GEN_OP(LEAVE),
    GEN_OP(SET),
    GEN_OP(CALL),
    GEN_OP(CALLNPR), // no preserve registers
    GEN_OP(CALLV), // varardic
    GEN_OP(CALLVNPR),
    GEN_OP(ADD),
    GEN_OP(SUB),
    GEN_OP(NEG),
    GEN_OP(MUL),
    GEN_OP(DIV),
    GEN_OP(CST),
    GEN_OP(EQ),
    GEN_OP(NE),
    GEN_OP(GT),
    GEN_OP(_END)
} gen_op; // not x64 opcodes, pseudo codes

const char *gen_op_str(gen_op go);

#define X64_TYPE(N) X64_TYPE_##N

typedef enum {
    X64_TYPE(N), // none
    X64_TYPE(M), // mem
    X64_TYPE(U3),
    X64_TYPE(U4),
    X64_TYPE(U5),
    X64_TYPE(U6),
    X64_TYPE(I3),
    X64_TYPE(I4),
    X64_TYPE(I5),
    X64_TYPE(I6),
    X64_TYPE(F5),
    X64_TYPE(F6)
} x64_type;

const char *x64_type_str(x64_type xt);

void gen_op_p(const tbl *ot, bool ci, size_t idnt);

void gen_p(const gen *g, const uint8_t *m);

te *gen_call_v(gen *g, vr *v);

te *gen_call_m(gen *g, size_t n, ...);

te *gen_lbl(gen *g, size_t id);

te *gen_arg(gen *g, x64_type t, size_t id);

te *gen_tmp(gen *g, x64_type t, size_t id);

te *gen_data(gen *g, x64_type t, un d);

te *gen_stkv(gen *g, x64_type t, size_t id);

// atm entry te[u5(id)|u4(cls)|u4(type);te[u4(cls)|u4(type);id];reg]

// lat entry te[u5(id)|u4(cls)|u4(type);te[u4(cls)|u4(type);id];code]

typedef struct {
    uint8_t rvc, xvc, rac, xac; // var count, arg count
    ssize_t r;
    const alfr *af, *ta;
    tbl *atm, *lat; // map args, tmp to regs, last code of arg, tmp
    vr *rstk, *xstk; // available regs and xmm
} gen_st;

gen_st *gen_st_i(const alfr *af, const alfr *ta, tbl *atm, tbl *lat, vr *rstk, vr *xstk);

gen_st *gen_st_i_gen_st(const gen_st *st);

void gen_st_p(const gen_st *st);

// pass 1
gen_stat gen_st_p1(gen *g, gen_st *st);

void set_code_s(te *ci, as *a);

void set_code_e(te *ci, as *a);

gen_stat rstk_b(const gen_st *st, uint8_t *r);

// get idx for stack var
gen_stat st_stkv_idx(const gen_st *st, x64_type t, uint8_t v, int16_t *idx);

void gen_as_rmbdr(as *a, as_inst i, reg d, int16_t dsp, reg s, te *ci);

void gen_as_rrmbd(as *a, as_inst i, reg d, reg s, int16_t dsp, te *ci);

gen_stat get_reg(gen_st *st, te *ovt, te **kv);

gen_stat get_reg_n(gen_st *st, te *ci, te **kv, size_t n);

// check if reg can be freed
void drop_atm_kv(gen_st *st, const te *atm_kv, const te *ci);

void drop_atm_kv_n(gen_st *st, te **atm_kv, const te *ci, size_t n);

void gen_st_f(gen_st *st);

as_stat gen_as(as *a, size_t op_id, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4, te *restrict ci);

#define AS(a, op_id, arg1, arg2, arg3, arg4, ci) gen_as(a, op_id, arg1, arg2, arg3, arg4, ci)
#define AS3(a, op_id, arg1, arg2, arg3, ci) AS(a, op_id, arg1, arg2, arg3, NULL, ci)
#define AS2(a, op_id, arg1, arg2, ci) AS3(a, op_id, arg1, arg2, NULL, ci)
#define AS1(a, op_id, arg1, ci) AS2(a, op_id, arg1, NULL, ci)
#define AS0(a, op_id, ci) AS1(a, op_id, NULL, ci)

gen_stat gen_err(const gen *g, te *ci, err **e, const char *m);

gen *gen_b(gen *g);
