
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

typedef struct {
    uint32_t tc, lc; // tmp counter, label counter
    ssize_t r;
    const alfr *af, *ta;
    atg_tbl_i *ati;
    gen *bg; // for base gens
    as *a;
    lst *q, *se; // queue of weak ref **an, start end fns
    tbl *at, *ot; // aply, op
} atg; // ast to gen

atg *atg_i(const alfr *af, const alfr *ta, atg_tbl_i ati, lst *q, lst *se, gen *g, as *a);

typedef bool atg_test_fn(const te *an);

atg_stat atg_q(atg *t, te **an, atg_test_fn enq);

// keep root node do not modify nodes
typedef atg_stat atg_cc_fn(atg *t, gen *g, te *an, te **e);

void atg_a_se(atg *t, atg_test_fn tse, atg_cc_fn s, atg_cc_fn e);

// te[u4(code)|u4(type)...;[tbl;cc]]
atg_stat atg_a_o(atg *t, uint16_t oc, type ct, ast_cls lc, type lt, ast_cls rc, type rt, atg_cc_fn cc);

atg_stat atg_qn(atg *t, gen **g, ast *a, te **e);

void atg_f(atg *t);
