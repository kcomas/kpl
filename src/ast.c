
#include "ast.h"

static ast_stat root(ast *a, te *pn, te **an) {
    *an = ast_t_i(a, NULL, pn, AST_CLS(R), P(a->sti()));
    if (!pn->d[2].p) return AST_STAT(OK);
    return ast_n(a, pn->d[2].p, (te**) &(*an)->d[4].p);
}

static ast_stat aply(ast *a, te *pn, te **an) {
    // TODO tgt
    // TODO lst
    *an = ast_t_i(a, *an, pn, AST_CLS(A), P(NULL), NULL, NULL);
    return AST_STAT(OK);
}

ast *ast_b(ast *a) {
    ast_a(a, NODE_TYPE(ROOT), root);
    ast_a(a, NODE_TYPE(APLY), aply);
    return a;
}

void ast_p(const te *t) {
    // TODO
}
