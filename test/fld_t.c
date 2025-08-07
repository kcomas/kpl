
#include "fld_t.h"

fld *bfld = NULL;

static __attribute__((constructor(104))) void chk_con(void) {
    bfld = fld_b(fld_i(&ast_am, &al_te, &al_err, ati, ali, NULL, mktbl(AST_CLS(_))));
}

static __attribute__((destructor)) void chk_des(void) {
    fld_f(bfld);
}

void fast(_tests *_t, ast *a, te **an, const fld *bf, bool rr) {
    E();
    fld *f = fld_i_fld(bf, a);
    err *e = NULL;
    fld_stat stat = fld_n(f, an, &e, rr);
    if (e) {
        err_p(e, 0, true);
        err_f(e);
    }
    A(stat == FLD_STAT(OK), "fld_n");
    fld_f(f);
}

// char *, un te[type]
tbl *fld_lst_tbl_i(ast *a, size_t n, ...) {
    tbl *t = ati();
    va_list args;
    va_start(args, n);
    while (n > 0) {
        mc *s = mc_i_cstr(va_arg(args, char*), &al_mc);
        un flgs = va_arg(args, un);
        te *type = va_arg(args, te*);
        te *e = ast_lst_tbl_e_i(a, s, flgs, type);
        tbl_a(t, e);
        n--;
    }
    va_end(args);
    return t;
}

// char*, te[type] id(if gid) ....
tbl *fld_type_tbl_i(bool gid, size_t n, ...) {
    tbl *t = ati();
    size_t id = 0;
    va_list args;
    va_start(args, n);
    for (size_t i = 0; i < n; i++) {
        mc *s = mc_i_cstr(va_arg(args, char*), &al_mc);
        te *type = va_arg(args, te*);
        if (gid) id = va_arg(args, size_t);
        type_tbl_a(t, &al_te, s, id, type);
    }
    va_end(args);
    return t;
}
