
#include "chk_t.h"

chk *bchk = NULL;

static __attribute__((constructor(105))) void chk_con(void) {
    bchk = chk_b(chk_i(&ast_am, &al_te, &al_lst, &al_tbl, &al_err, chk_err, cti, NULL));
}

static __attribute__((destructor)) void chk_des(void) {
    chk_f(bchk);
}

tbl *cti(void) {
    lst *tl = lst_i(&al_lst, &al_te, (void*) te_f);
    te *b = te_i(10, &al_te, NULL);
    return tbl_i(&al_tbl, tbl_no_hsh, tbl_un_eq, tl, b);
}

void rchk(_tests *_t, chk *c, te *an) {
    E();
    err *e = NULL;
    chk_stat stat = chk_n(c, an, &e);
    chk_f(c);
    if (e) {
        err_p(e, true);
        err_f(e);
    }
    A(stat == CHK_STAT(OK), "chk_n");
}
