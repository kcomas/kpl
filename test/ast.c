
#include "../src/ast.h"
#include "psr_t.h"
#include "t.h"

I;

const alfr am = { .a = malloc, .f = free };

static ssize_t pig(te *pn) {
    if (!pn || pn->l < 2) return -1;
    return pn->d[1].u6;
}

static tbl *ati(void) {
    lst *tl = lst_i(&am, &am, (void*) te_f);
    te *b = te_i(10, &am, NULL);
    return tbl_i(&am, tbl_sdbm, tbl_str_eq, tl, b);
}

static lst *ali(void) {
    return lst_i(&am, &am, (void*) te_f);
}

static tbl *mktbl(size_t size) {
    lst *tl = lst_i(&am, &am, (void*) te_f);
    te *b = te_i(size, &am, NULL);
    return tbl_i(&am, tbl_no_hsh, tbl_un_eq, tl, b);
}

static ast *ai(void) {
    return ast_b(ast_i(&am, &am, pig, ati, ali, mktbl(NODE_TYPE(_END)), mktbl(TCUST(_END))));
}

T(aplyopadd, {
    te *pn = psr_r(bpsr(aplyopadd)), *an = NULL;
    A(pn != NULL, "psr_r");
    ast *a = ai();
    A(ast_n(a, pn, (void**) &an) == AST_STAT(OK), "ast_n");
    ast_p(an);
    ast_f(a);
    te_f(an);
});
