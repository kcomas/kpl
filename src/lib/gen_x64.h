
#pragma once

#include "vr.h"
#include "gen.h"
#include "as_x64.h"

#define X64_TYPE(N) X64_TYPE_##N

typedef enum {
    X64_TYPE(N), // none
    X64_TYPE(S), // mc
    X64_TYPE(M), // *
    X64_TYPE(MU3),
    X64_TYPE(MU4),
    X64_TYPE(MU5),
    X64_TYPE(MU6),
    X64_TYPE(MI3),
    X64_TYPE(MI4),
    X64_TYPE(MI5),
    X64_TYPE(MI6),
    X64_TYPE(MF5),
    X64_TYPE(MF6),
    X64_TYPE(MM), // **
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

bool x64_type_is_ref(x64_type xt);

x64_type x64_type_to_ref(x64_type xt);

void gen_op_p(const tbl *ot, bool ci, size_t idnt);

void ovt_p(const te *ovt);

void gen_p(const gen *g, const uint8_t *m);

te *gen_call_v(gen *g, vr *v);

te *gen_call_m(gen *g, size_t n, ...);

vr *gen_vr(gen *g, size_t n, ...);

te *gen_call_w(gen *g, vr *s, vr *r);

te *gen_idx_v(gen *g, x64_type t, vr *v);

te *gen_idx_m(gen *g, x64_type t, size_t n, ...);

te *gen_lbl(gen *g, size_t id);

te *gen_arg(gen *g, x64_type t, size_t id);

te *gen_tmp(gen *g, x64_type t, size_t id);

te *gen_data(gen *g, x64_type t, un d);

te *gen_char(gen *g, const char *s);

te *gen_stkv(gen *g, x64_type t, size_t id);

te *gen_stka(gen *g, x64_type t, size_t id);

// atm entry te[u5(id)|u4(cls)|u4(type);te[u4(cls)|u4(type);id];reg]

// lat entry te[u5(id)|u4(cls)|u4(type);te[u4(cls)|u4(type);id];code]

bool gen_code_eq(const gen *a, const gen *b);

typedef struct {
    uint8_t rvc, xvc, rac, xac, rsc; // var count, arg count, stack count (only gen regs for stack)
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

// get idx for stack var
gen_stat st_stkv_idx(const gen_st *st, x64_type t, uint8_t v, int32_t *idx);

// get idx for stk arg
gen_stat st_stka_idx(x64_type t, uint8_t v, int32_t *idx);

te *bd_arg(as *a, int32_t dsp);

as_stat gen_as_rmbdr(as *a, as_inst i, reg d, int32_t dsp, reg s, te *ci);

as_stat gen_as_rrmbd(as *a, as_inst i, reg d, reg s, int32_t dsp, te *ci);

gen_stat get_reg(gen_st *st, te *ovt, te **kv);

gen_stat get_reg_n(gen_st *st, te *ci, te **kv, size_t n);

// check if reg can be freed
void drop_atm_kv(gen_st *st, const te *atm_kv, const te *ci);

void drop_atm_kv_n(gen_st *st, te **atm_kv, const te *ci, size_t n);

gen_stat stk_va(gen_st *st, te *restrict c, int32_t *restrict v);

gen_stat stk_g_idx2(gen_st *st, te *restrict c0, te *restrict c1, int32_t *restrict v0, int32_t *restrict v1);

gen_stat stk_g_idx3(gen_st *st, te *restrict c0, te *restrict c1, te *restrict c2, int32_t *restrict v0, int32_t *restrict v1, int32_t *restrict v2);

void gen_st_f(gen_st *st);

as_stat gen_as(as *a, size_t op_id, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4, te *restrict ci);

gen_stat gen_err(const gen *g, te *ci, err **e, const char *m);

void gen_x64_opt(gen *g, gen_st *st);

gen *gen_b(gen *g);
