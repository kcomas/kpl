
#include "ast.h"

static ast_stat root(ast *a, te *pn, void **vn) {
    te **an = (te**) vn;
    *an = ast_an_i(a, NULL, pn, AST_CLS(R), P(a->ati()));
    if (!pn->d[2].p) return AST_STAT(OK);
    return ast_n(a, pn->d[2].p, &(*an)->d[4].p);
}

static ast_stat i(ast *a, te *pn, void **vn) {
    te **an = (te**) vn;
    int64_t i = tkn_g_i6(pn->d[2].p, node_root_mc(pn));
    if (i < 0) return AST_STAT(INV);
    *an = ast_an_i(a, *an, pn, AST_CLS(S), P(type_s_i(a->ta, TYPE(I6))), I6(i));
    return AST_STAT(OK);
}

static ast_stat o(ast *a, te *pn, void **vn) {
    ast_stat stat;
    te **an = (te**) vn;
    size_t oid;
    if ((stat = ast_t_n(a, ((te*) pn->d[2].p)->d[0].u6, &oid)) != AST_STAT(OK)) return stat;
    *an = ast_an_i(a, *an, pn, AST_CLS(O), P(NULL), NULL, NULL, U6(oid));
    if (pn->d[3].p && (stat = ast_n(a, pn->d[3].p, &(*an)->d[4].p)) != AST_STAT(OK)) return stat;
    if (!pn->d[4].p) return AST_STAT(OK);
    return ast_n(a, pn->d[4].p, &(*an)->d[5].p);
}

static ast_stat aply(ast *a, te *pn, void **vn) {
    ast_stat stat;
    void *ln;
    te **an = (te**) vn;
    *an = ast_an_i(a, *an, pn, AST_CLS(A), P(NULL), NULL, NULL);
    if (pn->d[3].p && (stat = ast_n(a, pn->d[3].p, &(*an)->d[4].p)) != AST_STAT(OK)) return stat;
    if (!pn->d[4].p) return AST_STAT(OK);
    lst *pl = pn->d[4].p;
    if (pl->l == 0) return AST_STAT(OK);
    lst *al = a->ali();
    (*an)->d[5] = P(al);
    te *h = pl->h;
    while (h) {
        if ((stat = ast_n(a, h->d[0].p, &ln)) != AST_STAT(OK)) return stat;
        lst_ab(al, P(ln));
        h = h->d[2].p;
    }
    return AST_STAT(OK);
}

static ast *ast_tkn(ast *a) {
    ast_t_a(a, TCUST(ADD), OP(ADD));
    ast_t_a(a, TCUST(SUB), OP(SUB));
    return a;
}

ast *ast_b(ast *a) {
    ast_a(a, NODE_TYPE(ROOT), root);
    ast_a(a, NODE_TYPE(INT), i);
    ast_a(a, NODE_TYPE(OP), o);
    ast_a(a, NODE_TYPE(APLY), aply);
    return ast_tkn(a);
}

void ast_p(const te *an) {
    // TODO
}
