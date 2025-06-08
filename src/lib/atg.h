
#pragma once

#include "chk.h"
#include "gen.h"

#define ATG_STAT(N) ATG_STAT_##N

typedef enum {
    ATG_STAT(OK),
    ATG_STAT(INV)
} atg_stat;

typedef tbl *atg_tbl_i(void);

typedef lst *atg_lst_i(void);

typedef void atg_st_f(void *p);

typedef struct _atg atg;

typedef atg_stat atg_err_fn(const atg *t, te *an, err **e, const char *m);

typedef struct _atg {
    uint32_t tc, lc; // tmp counter, label counter
    ssize_t r;
    const alfr *af, *ta, *ea;
    atg_err_fn *efn;
    atg_tbl_i *ati;
    gen *bg; // for base gens
    as *a;
    lst *q, *se; // queue of weak ref **an, start end fns
    tbl *at, *ot; // aply, op
} atg; // ast to gen

atg *atg_i(const alfr *af, const alfr *ta, const alfr *ea, atg_err_fn efn, atg_tbl_i ati, lst *q, lst *se, gen *g, as *a);

atg *atg_i_atg(const atg *t);

typedef bool atg_test_fn(const te *an);

// use last node of ast for ref to its gen code
te *atg_g_g(te *an);

atg_stat atg_q(atg *t, te **an, atg_test_fn enq);

// keep root node do not modify nodes
typedef atg_stat atg_cc_fn(atg *t, gen *g, te *an, err **e);

void atg_a_se(atg *t, atg_test_fn tse, atg_cc_fn s, atg_cc_fn e);

// te[u4(code)|u4(type);cc]
atg_stat atg_a_a(atg *t, type rt, ast_cls tc, type tt, atg_cc_fn cc);

// te[u4(code)|u4(type)...;[tbl;cc]]
atg_stat atg_a_o(atg *t, uint16_t oc, type ct, ast_cls lc, type lt, ast_cls rc, type rt, atg_cc_fn cc);

atg_stat atg_lst_r(atg *t, gen *g, lst *l, err **e);

atg_stat atg_r(atg *t, gen *g, te *an, err **e);

atg_stat atg_n(atg *t, gen **g, ast *a, err **e);

void atg_f(atg *t);
