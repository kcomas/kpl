
#include "../src/ast.h"
#include "psr_t.h"
#include "t.h"

I;

const alfr am = { .a = malloc, .f = free };

static ssize_t pig(te *pn) {
    if (!pn || pn->l < 2) return -1;
    return pn->d[1].u6;
}

static tbl *sti(void) {
    lst *tl = lst_i(&am, &am, (void*) te_f);
    te *b = te_i(10, &am, NULL);
    return tbl_i(&am, tbl_sdbm, tbl_str_eq, tl, b);
}

static ast *ai(void) {
    lst *tl = lst_i(&am, &am, (void*) te_f);
    te *b = te_i(NODE_TYPE(_END), &am, NULL);
    tbl *t = tbl_i(&am, tbl_no_hsh, tbl_un_eq, tl, b);
    return ast_b(ast_i(&am, &am, pig, sti, t));
}

T(aplyopadd, {
    te *pn = psr_r(bpsr(aplyopadd)), *an = NULL;
    A(pn != NULL, "psr_r");
    ast *a = ai();
    A(ast_n(a, pn, &an) == AST_STAT(OK), "ast_n");
    ast_f(a);
    te_f(an);
});
