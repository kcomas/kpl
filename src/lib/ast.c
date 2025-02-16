
#include "ast.h"

ast *ast_i(const alfr *af, const alfr *ta, psr_id_g pig, ast_tbl_i ati, ast_lst_i ali, tbl *pt, tbl *tt) {
    ast *a = af->a(sizeof(ast));
    a->af = af;
    a->ta = ta;
    a->pig = pig;
    a->ati = ati;
    a->ali = ali;
    a->pt = pt;
    a->tt = tt;
    return a;
}

static void t_r_f(void *p) {
    te *n = p;
    te_f(n->d[1].p); // free psr
    tbl_f(n->d[3].p); // alias/export tbl
    te_f(n->d[5].p);
    n->af->f(n);
}

static void t_t_f(void *p) {
    te *n = p;
    te_f(n->d[3].p);
    n->af->f(n);
}

static void t_i_f(void *p) {
    te *n = p;
    te_f(n->d[3].p);
    te_f(n->d[4].p);
    n->af->f(n);
}

static void t_s_f(void *p) {
    te *n = p;
    te_f(n->d[3].p);
    // TODO data like sg
    n->af->f(n);
}

static void t_vl_f(void *p) {
    te *n = p;
    te_f(n->d[3].p);
    lst_f(n->d[4].p);
    n->af->f(n);
}

static void t_oz_f(void *p) {
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

te *ast_an_i(ast *a, te *restrict parent, te *restrict psr, ast_cls cls, un tt, ...) {
    size_t len = 5;
    frfn *nf = NULL;
    switch (cls) {
        case AST_CLS(R):
            len += 1;
            nf = t_r_f;
            break;
        case AST_CLS(T):
            nf = t_t_f;
            break;
        case AST_CLS(I):
            nf = t_i_f;
            break;
        case AST_CLS(S):
            nf = t_s_f;
            break;
        case AST_CLS(V):
            nf = t_vl_f;
            break;
        case AST_CLS(O):
            len += 2;
            nf = t_oz_f;
            break;
        case AST_CLS(Z):
            len += 1;
            nf = t_oz_f;
            break;
        case AST_CLS(A):
            len += 1;
            nf = t_a_f;
            break;
        case AST_CLS(L):
            nf = t_l_f;
            break;
        default:
            return NULL;
    }
    te *an = te_i(len, a->ta, nf);
    an->d[0] = P(parent);
    an->d[1] = P(psr);
    an->d[2] = U6(cls);
    an->d[3] = tt;
    va_list args;
    va_start(args, tt);
    for (size_t i = 4; i < len; i++) an->d[i] = va_arg(args, un);
    va_end(args);
    return an;
}

ast_stat ast_a(ast *a, size_t id, ast_tf atf) {
    te *kv;
    if (tbl_g_i(a->pt, U6(id), &kv) == TBL_STAT(OK)) return AST_STAT(INV);
    kv = te_i(2, a->ta, NULL);
    kv->d[0] = U6(id);
    kv->d[1] = P(atf);
    tbl_a(a->pt, kv);
    return AST_STAT(OK);
}

ast_stat ast_n(ast *a, te *pn, void **vn, te **e) {
    ast_stat stat;
    size_t pid;
    if ((stat = a->pig(pn, &pid)) != AST_STAT(OK)) return stat;
    te *kv;
    if (tbl_g_i(a->pt, U6(pid), &kv) == TBL_STAT(NF)) return AST_STAT(INV);
    ast_tf *atf = kv->d[1].p;
    return atf(a, pn, vn, e);
}

ast_stat ast_t_a(ast *a, size_t tid, size_t id) {
    te *kv;
    if (tbl_g_i(a->tt, U6(tid), &kv) == TBL_STAT(OK)) return AST_STAT(INV);
    kv = te_i(2, a->ta, NULL);
    kv->d[0] = U6(tid);
    kv->d[1] = U6(id);
    tbl_a(a->tt, kv);
    return AST_STAT(OK);
}

ast_stat ast_t_n(ast *a, size_t tid, size_t *id) {
    te *kv;
    if (tbl_g_i(a->tt, U6(tid), &kv) == TBL_STAT(NF)) return AST_STAT(INV);
    *id = kv->d[1].u6;
    return AST_STAT(OK);
}

void ast_f(ast *a) {
    tbl_f(a->pt);
    tbl_f(a->tt);
    a->af->f(a);
}
