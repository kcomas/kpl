
#pragma once

tbl *chk_tbl_i(void);

typedef struct {
    ssize_t r;
    const alfr *af, *ta;
    chk_tbl_i *cti;
    tbl *ct;
} chk;

void chk_f(chk *c);
