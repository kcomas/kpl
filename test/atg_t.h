
#pragma once

#include "../src/atg_x64.h"
#include "opt_t.h"
#include "gen_t.h"

void atg_des_verify(_tests *_t, atg *t, te *restrict tn, const void *fg, gen_st *st);

void atg_verify(_tests *_t, atg *t, ast *a, te *restrict an, te *restrict tn, const gen *gc, gen_st *st);

void atg_run(_tests *_t, atg *t, te *an, uint32_t elcmp, err **e);

#define D(TN, GC) atg_des_verify(_t, t, TN, GC, st)

#define V(CN, GC) atg_verify(_t, t, a, an, CN, GC, st)

#define AE(ELC) gen_st_f(st); \
    ast_f(a); \
    atg_run(_t, t, an, ELC, &e); \
    E(); \
    atg_f(t); \
    te_f(an); \


#define AR(ELC) AE(ELC); \
    A(e == NULL, "inv ret")

#define AI(PGM, DL, QL) IC(PGM); \
    RC(); \
    err *e = NULL; \
    fast(_t, a, &an, bopt, false); \
    atg *t = atg_i_atg(batg); \
    gen_st *st = gen_st_i(&am, &al_te, gen_op_tbl(20), gen_op_tbl(20), vr_i(16, &al_vr, NULL), vr_i(16, &al_vr, NULL)); \
    size_t cdl = t->dt->i->l; \
    bool okq = atg_q(t, &an, atg_x64_enq) == ATG_STAT(OK) && t->dt->i->l - cdl == DL && t->q->l == QL; \
    if (!okq) printf("D: %lu, L: %lu\n", t->dt->i->l - cdl, t->q->l); \
    A(okq, "atg_q")


