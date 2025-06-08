
#pragma once

#include "type.h"
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

extern const uint8_t chk_cls_conts[AST_CLS(_)];

typedef chk_stat chk_fn(chk *c, te *an, te **e);

#define CHK_HSH_C 1
#define CHK_HSH_T 0

// cls is also used for op codes
chk_stat chk_a(chk *c, chk_fn cf, uint16_t cls, uint16_t type, ...);

chk_stat chk_n(chk *c, te *an, te **e);

void chk_f(chk *c);
