
#pragma once

#include "type.h"
#include "fld.h"

#define CHK_STAT(N) CHK_STAT_##N

typedef enum {
    CHK_STAT(OK),
    CHK_STAT(INV)
} chk_stat;

typedef tbl *chk_tbl_i(void);

typedef struct _chk chk;

typedef chk_stat chk_err_fn(chk *c, te *an, err **e, const char *m);

// entry te[u4(cls)|u4(type);tbl...te[u4(cls)|u4(type);fn]]]

typedef struct _chk {
    int32_t fnlc; // fn label counter
    ssize_t r;
    const alfr *af, *ta, *ea;
    chk_err_fn *efn;
    chk_tbl_i *cti;
    ast *a;
    tbl *bt, *at; // before, after
} chk;

chk *chk_i(const alfr *af, const alfr *ta, const alfr *ea, chk_err_fn efn, chk_tbl_i cti, ast *a);

chk *chk_i_chk(const chk *c, ast *a);

extern const uint8_t chk_cls_conts[AST_CLS(_)];

typedef chk_stat chk_fn(chk *c, te *an, err **e);

// cls is also used for op codes
chk_stat chk_a(chk *c, tbl *t, chk_fn cf, uint16_t cls, uint16_t type, ...);

#define CHK_AB(C, CF, CLS, TYPE, ...) chk_a(C, C->bt, CF, CLS, TYPE, __VA_ARGS__)
#define CHK_AA(C, CF, CLS, TYPE, ...) chk_a(C, C->at, CF, CLS, TYPE, __VA_ARGS__)

chk_stat chk_n(chk *c, te *an, err **e);

void chk_f(chk *c);
