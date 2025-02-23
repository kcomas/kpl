
#include "../src/fld.h"
#include "ast_t.h"

static void fast(_tests *_t, te **an) {
    E;
    fld_stat stat;
    fld *f = fld_b(fld_i(&ast_am, &ast_am, ati, ali, mktbl(AST_CLS(_))));
    te *e = NULL;
    stat = fld_n(f, an, &e);
    if (e) {
        ast_p(e, 0);
        putchar('\n');
    }
    A(stat == FLD_STAT(OK), "fld_n");
    fld_f(f);
}

// char*, te[type] ....
static tbl *type_tbl_i(size_t n, ...) {
    tbl *t = ati();
    va_list args;
    va_start(args, n);
    while (n > 0) {
        mc *s = mc_i_cstr(va_arg(args, char*), &ast_am);
        te *type = va_arg(args, te*);
        type_tbl_a(t, &ast_am, s, type);
        n--;
    }
    va_end(args);
    return t;
}

#define V(PGM, AST) ast *a = ast_b(ast_i(&ast_am, &ast_am, &ast_am, pig, ali, mktbl(NODE_TYPE(_END)), mktbl(TCUST(_END)))); \
    te *an = NULL; \
    bast(_t, a, PGM, &an); \
    fast(_t, &an); \
    ast_verify(_t, a, an, AST)

T(aplyopadd) {
    V(aplyopadd, RN(ON(P(NULL), ADD, SN(I6, I6(1)), SN(I6, I6(2)))));
}

T(typetype) {
    V(typetype, NULL);
}
