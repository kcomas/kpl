
#pragma once

tbl *chk_tbl_i(void);

// entry te[cls;tbl[cls_chk;tbl...;te[cls_ck;fn]]]

typedef struct {
    ssize_t r;
    const alfr *af, *ta;
    frfn *cef;
    chk_tbl_i *cti;
    ast *a;
    tbl *ct;
} chk;

chk *chk_i(const alfr *af, const alfr *ta, frfn *cef, chk_tbl_i cti, ast *a);

void chk_f(chk *c);
