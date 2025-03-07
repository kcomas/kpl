
#include "fld_t.h"

void fast(_tests *_t, ast *a, te **an, fld_build_fn fn, bool rr) {
    E();
    fld *f = fn(fld_i(&ast_am, &ast_am, ati, ali, a, mktbl(AST_CLS(_))));
    te *e = NULL;
    fld_stat stat = fld_n(f, an, &e, rr);
    if (e) {
        ast_p(e, 0);
        printf("\n rr: %d\n", rr);
    }
    A(stat == FLD_STAT(OK), "fld_n");
    fld_f(f);
}

// char *, te[type] un(flags)
tbl *fld_lst_tbl_i(ast *a, size_t n, ...) {
    tbl *t = ati();
    va_list args;
    va_start(args, n);
    while (n > 0) {
        mc *s = mc_i_cstr(va_arg(args, char*), &ast_am);
        un flgs = va_arg(args, un);
        te *type = va_arg(args, te*);
        te *e = ast_lst_tbl_e_i(a, s, flgs, type);
        tbl_a(t, e);
        n--;
    }
    va_end(args);
    return t;
}

// char*, te[type] ....
tbl *fld_type_tbl_i(size_t n, ...) {
    tbl *t = ati();
    va_list args;
    va_start(args, n);
    for (size_t i = 0; i < n; i++) {
        mc *s = mc_i_cstr(va_arg(args, char*), &ast_am);
        te *type = va_arg(args, te*);
        type_tbl_a(t, &ast_am, s, i, type);
    }
    va_end(args);
    return t;
}
