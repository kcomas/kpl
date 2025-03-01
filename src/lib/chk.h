
#pragma once

#include "ast.h"

#define CHK_STAT(N) CHK_STAT_##N

typedef enum {
    CHK_STAT(OK),
    CHK_STAT(INV)
} chk_stat;

typedef tbl *chk_tbl_i(void);

// entry te[u4(cls)|u4(type);tbl...te[u4(cls)|u4(type);fn]]]

typedef struct {
    ssize_t r;
    const alfr *af, *ta;
    chk_tbl_i *cti;
    ast *a;
    tbl *ct;
} chk;

chk *chk_i(const alfr *af, const alfr *ta, chk_tbl_i cti, ast *a);

extern const uint8_t chk_cls_conts[];

typedef chk_stat chk_fn(chk *c, te *an);

// cls is also used for ops, cmds
void chk_a(chk *c, chk_fn cf, uint16_t cls, uint16_t type, ...);

void chk_f(chk *c);
