
#include "ast.h"

const char *ast_cls_str(ast_cls cls) {
    switch (cls) {
        case AST_CLS(R): return "R";
        case AST_CLS(I): return "I";
        case AST_CLS(S): return "S";
        case AST_CLS(V): return "V";
        case AST_CLS(O): return "O";
        case AST_CLS(T): return "T";
        case AST_CLS(A): return "A";
        case AST_CLS(L): return "L";
        default:
            break;
    }
    return "INV";
}

ast *ast_i(const alfr *af, const alfr *na, psr_id_g pig, tbl *t) {
    ast *a = af->a(sizeof(ast));
    a->af = af;
    a->na = na;
    a->pig = pig;
    a->t = t;
    return a;
}

static void t_r_f(void *p) {
    te *n = p;
    te_f(n->d[0].p); // free psr
    tbl_f(n->d[3].p);
    n->af->f(n);
}

static void t_i_f(void *p) {
    te *n = p;
    te_f(n->d[3].p);
    te_f(n->d[4].p);
    n->af->f(n);
}

static void t_vl_f(void *p) {
    te *n = p;
    te_f(n->d[3].p);
    lst_f(n->d[4].p);
    n->af->f(n);
}

static void t_ot_f(void *p) {
    te *n = p;
    te_f(n->d[3].p);
    te_f(n->d[4].p);
    te_f(n->d[5].p);
    n->af->f(n);
}

static void t_a_f(void *p) {
    te *n = p;
    te_f(n->d[3].p);
    te_f(n->d[4].p);
    lst_f(n->d[5].p);
    n->af->f(n);
}

static void t_l_f(void *p) {
    te *n = p;
    tbl_f(n->d[3].p);
    lst_f(n->d[4].p);
    n->af->f(n);
}

te *ast_t_i(ast *a, te *restrict parent, te *restrict psr, ast_cls cls, un tt, ...) {
    size_t len = 4;
    frfn *nf = NULL;
    switch (cls) {
        case AST_CLS(R):
            nf = t_r_f;
            break;
        case AST_CLS(I):
            len += 1;
            nf = t_i_f;
            break;
        case AST_CLS(S):
            len += 1;
            break;
        case AST_CLS(V):
            len += 1;
            nf = t_vl_f;
            break;
        case AST_CLS(O):
            len += 3;
            nf = t_ot_f;
            break;
        case AST_CLS(T):
            len += 2;
            nf = t_ot_f;
            break;
        case AST_CLS(A):
            len += 2;
            nf = t_a_f;
            break;
        case AST_CLS(L):
            len += 1;
            nf = t_l_f;
            break;
    }
    if (len == 4) return NULL;
    te *t = te_i(len, a->na, nf);
    t->d[0] = P(parent);
    t->d[1] = P(psr);
    t->d[2] = U6(cls);
    t->d[3] = tt;
    va_list args;
    va_start(args, tt);
    for (size_t i = 4; i < len; i++) t->d[i] = va_arg(args, un);
    va_end(args);
    return t;
}

ast_stat ast_a(ast *a, size_t id, ast_tf atf) {
    te *kv;
    if (tbl_g_i(a->t, U6(id), &kv) == TBL_STAT(OK)) return AST_STAT(INV);
    kv = te_i(2, a->na, NULL);
    kv->d[0] = U6(id);
    kv->d[1] = P(atf);
    tbl_a(a->t, kv);
    return AST_STAT(OK);
}

ast_stat ast_n(ast *a, te *pn, te **an) {
    ssize_t pid = a->pig(pn);
    if (pid < 0) return AST_STAT(INV);
    te *kv;
    if (tbl_g_i(a->t, U6(pid), &kv) == TBL_STAT(NF)) return AST_STAT(INV);
    ast_tf *atf = kv->d[1].p;
    return atf(a, pn, an);
}

void ast_f(ast *a) {
    tbl_f(a->t);
    a->af->f(a);
}
