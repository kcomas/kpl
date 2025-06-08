
#include "ast.h"

static ast_stat root(ast *a, te *pn, void **vn) {
    te **an = (te**) vn;
    *an = ast_an_i(a, NULL, pn, AST_CLS(R), P(a->ati()));
    if (!pn->d[2].p) return AST_STAT(OK);
    return ast_n(a, pn->d[2].p, &(*an)->d[4].p);
}

static ast_stat i(ast *a, te *pn, void **vn) {
    te **an = (te**) vn;
    // TODO
    return AST_STAT(INV);
}

static ast_stat op(ast *a, te *pn, void **vn) {
    te **an = (te**) vn;
    // TODO
    return AST_STAT(INV);
}

static ast_stat aply(ast *a, te *pn, void **vn) {
    ast_stat stat;
    void *ln;
    te **an = (te**) vn;
    *an = ast_an_i(a, *an, pn, AST_CLS(A), P(NULL), NULL, P(NULL));
    if (pn->d[3].p && (stat = ast_n(a, pn->d[3].p, &(*an)->d[4].p)) != AST_STAT(OK)) return stat;
    if (!pn->d[4].p) return AST_STAT(OK);
    lst *pl = pn->d[4].p;
    if (pl->l == 0) return AST_STAT(OK);
    lst *al = a->ali();
    (*an)->d[5].p = al;
    te *h = pl->h;
    while (h) {
        if ((stat = ast_n(a, h->d[0].p, &ln)) != AST_STAT(OK)) return stat;
        lst_ab(al, P(ln));
        h = h->d[2].p;
    }
    return AST_STAT(OK);
}

ast *ast_b(ast *a) {
    ast_a(a, NODE_TYPE(ROOT), root);
    ast_a(a, NODE_TYPE(INT), i);
    ast_a(a, NODE_TYPE(OP), op);
    ast_a(a, NODE_TYPE(APLY), aply);
    return a;
}

void ast_p(const te *an) {
    // TODO
}
