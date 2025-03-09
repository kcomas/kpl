
#pragma once

#include "ast.h"
#include "gen.h"

#define ATG_STAT(N) ATG_STAT_##N

typedef enum {
    ATG_STAT(OK),
    ATG_STAT(INV)
} atg_stat;

typedef tbl *atg_tbl_i(void);

typedef struct {
    uint32_t lc; // label counter
    ssize_t r;
    const alfr *af, *ta, *saf, *sta; // alloc for gen st
    atg_tbl_i *aoti;
    gen *g; // for base gens
    as *a;
    lst *q; // queue
    tbl *at, *ot; // not op, op
} atg; // ast to gen

atg *atg_i(const alfr *af, const alfr *ta, const alfr *saf, const alfr *sta, atg_tbl_i *aoti, gen *g, as *a, lst *q);

typedef bool atg_enq(const te *an);

atg_stat atg_q(atg *t, te *an, atg_enq enq);

void atg_f(atg *t);
