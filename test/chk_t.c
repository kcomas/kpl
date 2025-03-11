
#include "chk_t.h"

tbl *cti(void) {
    lst *tl = lst_i(&ast_am, &ast_am, (void*) te_f);
    te *b = te_i(10, &ast_am, NULL);
    return tbl_i(&ast_am, tbl_no_hsh, tbl_un_eq, tl, b);
}

void rchk(_tests *_t, chk *c, te *an) {
    E();
    te *e = NULL;
    chk_stat stat = chk_n(c, an, &e);
    chk_f(c);
    if (e) {
        ast_p(e, 0);
        putchar('\n');
    }
    A(stat == CHK_STAT(OK), "chk_n");
}
