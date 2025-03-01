
#include "../src/chk.h"
#include "fld_t.h"

#define V(PGM, AST) ast *a = ast_b(ast_i(&ast_am, &ast_am, &ast_am, pig, ali, mktbl(NODE_TYPE(_END)), mktbl(TCUST(_END)))); \
    te *an = NULL; \
    bast(_t, a, PGM, &an); \
    fast(_t, a, &an); \
    ast_verify(_t, a, an, AST)

T(fnadd3) {
    ast *a = ast_b(ast_i(&ast_am, &ast_am, &ast_am, pig, ali, mktbl(NODE_TYPE(_END)), mktbl(TCUST(_END))));
    te *an = NULL;
    bast(_t, a, fnadd3, &an);
    fast(_t, a, &an);
    chk *c = chk_i(&ast_am, &ast_am, ati, a);
    chk_f(c);
    ast_f(a);
    te_f(an);
}
