
#include "../src/chk.h"
#include "fld_t.h"

static tbl *cti(void) {
    lst *tl = lst_i(&ast_am, &ast_am, (void*) te_f);
    te *b = te_i(10, &ast_am, NULL);
    return tbl_i(&ast_am, tbl_no_hsh, tbl_un_eq, tl, b);
}

T(fnadd3) {
    ast *a = ast_b(ast_i(&ast_am, &ast_am, &ast_am, pig, ali, mktbl(NODE_TYPE(_END)), mktbl(TCUST(_END))));
    te *an = NULL;
    bast(_t, a, fnadd3, &an);
    fast(_t, a, &an);
    chk *c = chk_i(&ast_am, &ast_am, cti, a);
    chk_b(c);
    chk_p(c->ct, 0);
    chk_f(c);
    ast_f(a);
    te_f(an);
}
