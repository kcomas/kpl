
#pragma once

#include "ast.h"

#define CHK_STAT(N) CHK_STAT_##N

typedef enum {
    CHK_STAT(OK),
    CHK_STAT(INV)
} chk_stat;

typedef tbl *chk_tbl_i(void);

// entry te[cls;tbl[cls_chk;tbl...;te[cls_ck;fn]]]

typedef struct {
    ssize_t r;
    const alfr *af, *ta;
    chk_tbl_i *cti;
    ast *a;
    tbl *ct;
} chk;

chk *chk_i(const alfr *af, const alfr *ta, chk_tbl_i cti, ast *a);

typedef chk_stat chk_fn(chk *c, te *an);

void chk_a(chk *c, ast_cls cls, un ct, ...);

void chk_f(chk *c);
