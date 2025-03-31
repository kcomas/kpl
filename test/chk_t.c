
#include "chk_t.h"

chk *bchk = NULL;

static __attribute__((constructor)) void chk_con(void) {
    bchk = chk_b(chk_i(&ast_am, &ast_am, &ast_am, ast_err_p, cti, NULL));
}

static __attribute__((destructor)) void chk_des(void) {
    chk_f(bchk);
}

tbl *cti(void) {
    lst *tl = lst_i(&ast_am, &ast_am, (void*) te_f);
    te *b = te_i(10, &ast_am, NULL);
    return tbl_i(&ast_am, tbl_no_hsh, tbl_un_eq, tl, b);
}

void rchk(_tests *_t, chk *c, te *an) {
    E();
    err *e = NULL;
    chk_stat stat = chk_n(c, an, &e);
    chk_f(c);
    if (e) {
        err_p(e);
        err_f(e);
    }
    A(stat == CHK_STAT(OK), "chk_n");
}
