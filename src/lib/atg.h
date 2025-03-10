
#pragma once

#include "ast.h"
#include "gen.h"

#define ATG_STAT(N) ATG_STAT_##N

typedef enum {
    ATG_STAT(OK),
    ATG_STAT(INV)
} atg_stat;

typedef tbl *atg_tbl_i(void);

typedef lst *atg_lst_i(void);

typedef struct {
    uint32_t lc; // label counter
    ssize_t r;
    const alfr *af, *ta, *saf, *sta; // alloc for gen st
    atg_tbl_i *aoti;
    atg_lst_i *ali;
    gen *g; // for base gens
    as *a;
    lst *q, *se; // queue of weak ref **an, start end fns
    tbl *at, *ot; // not op, op
} atg; // ast to gen

atg *atg_i(const alfr *af, const alfr *ta, const alfr *saf, const alfr *sta, atg_tbl_i *aoti, atg_lst_i ali, gen *g, as *a);

typedef bool atg_test_fn(const te *an);

atg_stat atg_q(atg *t, te *an, atg_test_fn enq);

// current return type
typedef atg_stat atg_cc_fn(atg *t, const te *an, te **e);

atg_stat atg_qn(atg *t);

void atg_f(atg *t);
