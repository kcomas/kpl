
#include "../src/ast.h"
#include "psr_t.h"
#include "t.h"

static const alfr am = { .a = malloc, .f = free };

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

#define RN(N) ast_an_i(a, NULL, NULL, AST_CLS(R), P(NULL), N)

#define TN(V) ast_an_i(a, NULL, NULL, AST_CLS(T), P(type_i(&am, TYPE(V))))

#define SN(T, V) ast_an_i(a, NULL, NULL, AST_CLS(S), P(type_i(&am, TYPE(T))), V)

#define ON(T, C, L, R) ast_an_i(a, NULL, NULL, AST_CLS(O), T, OC(C), L, R)

static te *aply(ast *a, un type, te *tgt, size_t n, ...) {
    lst *l = ali();
    va_list arg;
    va_start(arg, n);
    while (n > 0) {
        lst_ab(l, P(va_arg(arg, te*)));
        n--;
    }
    va_end(arg);
    return ast_an_i(a, NULL, NULL, AST_CLS(A), type, tgt, l);
}

#define AN(T, TGT, N, ...) aply(a, T, TGT, N, __VA_ARGS__)

#define ZN(S, TGT) ast_an_i(a, NULL, NULL, AST_CLS(Z), P(type_i(&am, TYPE(SL))), mc_i_cstr(S, &am), TGT)

static void ast_verify(_tests *_t, ast *a, const char *pgm, te *cn) {
    printf("%s\n", pgm);
    te *pn = psr_r(bpsr(pgm)), *an = NULL, *e = NULL;
    A(pn != NULL, "psr_r");
    A(ast_n(a, pn, (void**) &an, &e) == AST_STAT(OK), "ast_n");
    ast_p(an, 0);
    putchar('\n');
    ast_p(cn, 0);
    putchar('\n');
    A(ast_eq(an, cn), "ast_eq");
    ast_f(a);
    te_f(an);
    te_f(cn);
}

#define V(PGM, AST) ast *a = ast_b(ast_i(&am, &am, &am, pig, ati, ali, mktbl(NODE_TYPE(_END)), mktbl(TCUST(_END)))); \
    ast_verify(_t, a, PGM, AST)

T(ast_aplyopadd) {
    V(aplyopadd, RN(AN(P(NULL), ON(P(NULL), ADD, NULL, NULL), 2, SN(I6, I6(1)), SN(I6, I6(2)))));
}

T(ast_typetype) {
    V(typetype, RN(AN(P(NULL), TN(FN), 3, ZN("x", TN(I6)), ZN("y", TN(F6)), AN(P(NULL), TN(FN), 2, ZN("z", TN(U6)), TN(I6)))));
}
