
#include "../src/fld.h"
#include "ast_t.h"

static void fast(_tests *_t, te **an) {
    fld *f = fld_b(fld_i(&ast_am, &ast_am, ati, ali, mktbl(AST_CLS(_))));
    A(fld_n(f, an) == FLD_STAT(OK), "fld_n");
    fld_f(f);
}

#define V(PGM, AST) ast *a = ast_b(ast_i(&ast_am, &ast_am, &ast_am, pig, ali, mktbl(NODE_TYPE(_END)), mktbl(TCUST(_END)))); \
    te *an = NULL; \
    bast(_t, a, PGM, &an); \
    fast(_t, &an); \
    ast_verify(_t, a, an, AST)

T(ast_aplyopadd) {
    V(aplyopadd, RN(ON(P(NULL), ADD, SN(I6, I6(1)), SN(I6, I6(2)))));
}
