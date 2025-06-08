
#include "ast.h"

static void node_vl_f(void *p) {
    te *n = p;
    lst_f(n->d[4].p);
    n->af->f(n);
}

static void node_o_f(void *p) {
    te *n = p;
    te_f(n->d[4].p);
    te_f(n->d[5].p);
    n->af->f(n);
}

static void node_t_f(void *p) {
    te *n = p;
    te_f(n->d[4].p);
    n->af->f(n);
}

static void node_a_f(void *p) {
    te *n = p;
    te_f(n->d[4].p);
    lst_f(n->d[5].p);
    n->af->f(n);
}

te *ast_node_i(const alfr *ta, te *restrict parent, te *restrict psr, ast_cls cls, un type, ...) {
    size_t len = 4;
    frfn *nf = NULL;
    switch (cls) {
        case AST_CLS(S):
            len += 1;
            break;
        case AST_CLS(V):
            len += 1;
            nf = node_vl_f;
            break;
        case AST_CLS(O):
            len += 2;
            nf = node_o_f;
            break;
        case AST_CLS(T):
            len += 1;
            nf = node_t_f;
            break;
        case AST_CLS(A):
            len += 2;
            nf = node_a_f;
            break;
        case AST_CLS(L):
            len += 1;
            nf = node_vl_f;
            break;
    }
    if (len == 4) return NULL;
    te *n = te_i(len, ta, nf);
    n->d[0] = P(parent);
    n->d[1] = P(psr);
    n->d[2] = U6(cls);
    n->d[3] = type;
    va_list a;
    va_start(a, type);
    for (size_t i = 4; i < len; i++) n->d[i] = va_arg(a, un);
    va_end(a);
    return n;
}
