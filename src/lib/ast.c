
#include "ast.h"

ast *ast_i(const alfr *af, const alfr *na, tbl *t) {
    ast *a = af->a(sizeof(ast));
    a->af = af;
    a->na = na;
    a->t = t;
    return a;
}

static void t_vl_f(void *p) {
    te *n = p;
    lst_f(n->d[4].p);
    n->af->f(n);
}

static void t_o_f(void *p) {
    te *n = p;
    te_f(n->d[4].p);
    te_f(n->d[5].p);
    n->af->f(n);
}

static void t_t_f(void *p) {
    te *n = p;
    te_f(n->d[4].p);
    n->af->f(n);
}

static void t_a_f(void *p) {
    te *n = p;
    te_f(n->d[4].p);
    lst_f(n->d[5].p);
    n->af->f(n);
}

te *ast_t_i(ast *a, te *restrict parent, te *restrict psr, ast_cls cls, un type, ...) {
    size_t len = 4;
    frfn *nf = NULL;
    switch (cls) {
        case AST_CLS(S):
            len += 1;
            break;
        case AST_CLS(V):
            len += 1;
            nf = t_vl_f;
            break;
        case AST_CLS(O):
            len += 2;
            nf = t_o_f;
            break;
        case AST_CLS(T):
            len += 1;
            nf = t_t_f;
            break;
        case AST_CLS(A):
            len += 2;
            nf = t_a_f;
            break;
        case AST_CLS(L):
            len += 1;
            nf = t_vl_f;
            break;
    }
    if (len == 4) return NULL;
    te *t = te_i(len, a->na, nf);
    t->d[0] = P(parent);
    t->d[1] = P(psr);
    t->d[2] = U6(cls);
    t->d[3] = type;
    va_list args;
    va_start(args, type);
    for (size_t i = 4; i < len; i++) t->d[i] = va_arg(args, un);
    va_end(args);
    return t;
}

void ast_f(ast *a) {
    tbl_f(a->t);
    a->af->f(a);
}
