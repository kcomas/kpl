
#include "ast_t.h"

const alfr ast_am = { .a = malloc, .f = free };

ast *bast = NULL;

static __attribute__((constructor)) void ast_con(void) {
    bast = ast_b(ast_i(&ast_am, &ast_am, &ast_am, &ast_am, ast_err_p, pig, ali, mktbl(NODE_TYPE(_END)), mktbl(TCUST(_END))));
}

static __attribute__((destructor)) void ast_des(void) {
    ast_f(bast);
}

ast_stat pig(te *pn, size_t *pid) {
    if (!pn || pn->l < 2) return AST_STAT(INV);
    *pid = pn->d[1].u6;
    return AST_STAT(OK);
}

tbl *ati(void) {
    lst *tl = lst_i(&ast_am, &ast_am, (void*) te_f);
    te *b = te_i(10, &ast_am, NULL);
    return tbl_i(&ast_am, tbl_mc_sdbm, tbl_mc_eq, tl, b);
}

lst *ali(void) {
    return lst_i(&ast_am, &ast_am, (void*) te_f);
}

tbl *mktbl(size_t size) {
    lst *tl = lst_i(&ast_am, &ast_am, (void*) te_f);
    te *b = te_i(size, &ast_am, NULL);
    return tbl_i(&ast_am, tbl_no_hsh, tbl_un_eq, tl, b);
}

void astb(_tests *_t, ast *a, const char *pgm, te **an) {
    E();
    printf("%s\n", pgm);
    err *e = NULL;
    te *pn = psr_r(bpsr(pgm));
    A(pn != NULL, "psr_r");
    ast_stat stat = ast_n(a, NULL, pn, (void**) an, &e);
    if (e) {
        err_p(e);
        err_f(e);
    }
    A(stat == AST_STAT(OK), "ast_n");
}

void ast_verify(_tests *_t, ast *a, te *restrict an, te *restrict tn) {
    E();
    ast_p(an, 0);
    putchar('\n');
    ast_p(tn, 0);
    putchar('\n');
    A(ast_eq(an, tn), "ast_eq");
    ast_f(a);
    te_f(an);
    te_f(tn);
}
