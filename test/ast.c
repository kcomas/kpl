
#include "../src/ast.h"
#include "psr_t.h"
#include "t.h"

I;

const alfr am = { .a = malloc, .f = free };

static ast_stat pig(te *pn, size_t *pid) {
    if (!pn || pn->l < 2) return AST_STAT(INV);
    *pid = pn->d[1].u6;
    return AST_STAT(OK);
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

#define ROOT(N) ast_an_i(a, NULL, NULL, AST_CLS(R), P(NULL), N)

#define SCALAR(T, V) ast_an_i(a, NULL, NULL, AST_CLS(S), P(type_s_i(am, TYPE(T))), V)

#define OP(C, L, R) ast_an_i(a, NULL, NULL, AST_CLS(O), L, R, OP(C))

T(aplyopadd, {
    printf("%s\n", aplyopadd);
    te *pn = psr_r(bpsr(aplyopadd)), *an = NULL, *e = NULL;
    A(pn != NULL, "psr_r");
    ast *a = ai();
    A(ast_n(a, pn, (void**) &an, &e) == AST_STAT(OK), "ast_n");
    ast_p(an, 0);
    putchar('\n');
    te *cmp = ROOT(NULL);
    A(ast_eq(an, cmp), "ast_eq");
    ast_f(a);
    te_f(an);
    te_f(cmp);
});
