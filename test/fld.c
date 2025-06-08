
#include "../src/fld.h"
#include "ast_t.h"

static void fast(_tests *_t, ast *a, te **an) {
    E();
    fld_stat stat;
    fld *f = fld_b(fld_i(&ast_am, &ast_am, ati, ali, a, mktbl(AST_CLS(_))));
    te *e = NULL;
    stat = fld_n(f, an, &e);
    if (e) {
        ast_p(e, 0);
        putchar('\n');
    }
    A(stat == FLD_STAT(OK), "fld_n");
    fld_f(f);
}

static tbl *lst_tbl_i(ast *a, size_t n, ...) {
    tbl *t = ati();
    va_list args;
    va_start(args, n);
    while (n > 0) {
        tbl_a(t, ast_lst_tbl_e_i(a, mc_i_cstr(va_arg(args, char*), &ast_am)));
        n--;
    }
    va_end(args);
    return t;
}

#define LT(N, ...) lst_tbl_i(a, N, __VA_ARGS__)

// char*, te[type] ....
static tbl *type_tbl_i(size_t n, ...) {
    tbl *t = ati();
    va_list args;
    va_start(args, n);
    for (size_t i = 0; i < n; i++) {
        mc *s = mc_i_cstr(va_arg(args, char*), &ast_am);
        te *type = va_arg(args, te*);
        type_tbl_a(t, &ast_am, s, i, type);
    }
    va_end(args);
    return t;
}

#define TF(FT, RT, N, ...) type_f_i(&ast_am, TYPE(FT), RT, type_tbl_i(N, __VA_ARGS__))

#define TS(T) type_s_i(&ast_am, TYPE(T))

#define V(PGM, AST) ast *a = ast_b(ast_i(&ast_am, &ast_am, &ast_am, pig, ali, mktbl(NODE_TYPE(_END)), mktbl(TCUST(_END)))); \
    te *an = NULL; \
    bast(_t, a, PGM, &an); \
    fast(_t, a, &an); \
    ast_verify(_t, a, an, AST)

T(aplyopadd) {
    V(aplyopadd, RN(ON(NULL, ADD, SN(I6, I6(1)), SN(I6, I6(2)))));
}

T(typetype) {
    V(typetype, RN(NN(T, TF(FN, TF(FN, TS(I6), 1, "z", TS(U6)), 2, "x", TS(I6), "y", TS(F6)))));
}

T(fnadd3) {
    V(fnadd3, RN(LN(LT(1, "f"), L(2,
        ON(NULL, AGN, EN("f"),
            ON(NULL, CST,
            NN(T, TF(FN, TS(I6), 3, "a", TS(I6), "b", TS(I6), "c", TS(I6))),
            LN(LT(3, "a", "b", "c"), L(1, ON(NULL, SUB, NULL, ON(NULL, ADD, EN("a"), ON(NULL, ADD, EN("b"), EN("c"))))))
        )),
        AN(NULL, EN("f"), L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3))))))));
}
