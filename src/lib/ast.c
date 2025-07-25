
#include "ast.h"

const char *ast_cls_str(ast_cls cls) {
    switch (cls) {
        case AST_CLS(R): return "R";
        case AST_CLS(T): return "T";
        case AST_CLS(E): return "E";
        case AST_CLS(I): return "I";
        case AST_CLS(S): return "S";
        case AST_CLS(V): return "V";
        case AST_CLS(O): return "O";
        case AST_CLS(Z): return "Z";
        case AST_CLS(A): return "A";
        case AST_CLS(L): return "L";
        case AST_CLS(C): return "C";
        case AST_CLS(_): return "_";
        default:
            break;
    }
    return "INV CLS";
}

ast *ast_i(const alfr *af, const alfr *ta, const alfr *ma, const alfr *ea, err_d_p edp, psr_id_g pig, ast_lst_i ali, tbl *pt, tbl *tt) {
    ast *a = af->a(sizeof(ast));
    a->r = 1;
    a->af = af;
    a->ta = ta;
    a->ma = ma;
    a->ea = ea;
    a->edp = edp;
    a->pig = pig;
    a->ali = ali;
    a->pt = pt;
    a->tt = tt;
    return a;
}

ast *ast_i_ast(const ast *a) {
    return ast_i(a->af, a->ta, a->ma, a->ea, a->edp, a->pig, a->ali, tbl_c(a->pt), tbl_c(a->tt));
}

ast *ast_c(ast *a) {
    a->r++;
    return a;
}

un ast_hsh(const te *an) {
    un hsh = U6(0);
    if (!an) {
        hsh = u4_s_o(hsh, AST_HSH_C, AST_CLS(_));
        hsh = u4_s_o(hsh, AST_HSH_T, TYPE(_N));
        return hsh;
    }
    hsh = u4_s_o(hsh, AST_HSH_C, an->d[2].u4);
    switch (an->d[2].u4) {
        case AST_CLS(R):
        case AST_CLS(L):
            hsh = u4_s_o(hsh, AST_HSH_T, TYPE(_A));
            break;
        case AST_CLS(E):
        case AST_CLS(Z):
            if (!((te*) an->d[3].p)->d[2].p) hsh = u4_s_o(hsh, AST_HSH_T, TYPE(_N));
            else hsh = u4_s_o(hsh, AST_HSH_T, ((te*) ((te*) an->d[3].p)->d[2].p)->d[1].u4);
            break;
        default:
            if (!an->d[3].p) hsh = u4_s_o(hsh, AST_HSH_T, TYPE(_N));
            else hsh = u4_s_o(hsh, AST_HSH_T, ((te*) an->d[3].p)->d[1].u4);
            break;
    }
    return hsh;
}

ast_stat ast_g_pn(ast_cls cls, te *an, te **pn) {
    while (an) {
        if (an->d[2].u4 == cls) {
            *pn = an;
            return AST_STAT(OK);
        }
        an = an->d[0].p;
    }
    return AST_STAT(INV);
}

te *ast_g_root(te *an) {
    te *rn = NULL;
    ast_g_pn(AST_CLS(R), an, &rn);
    return rn;
}

ast_stat ast_g_t(te *an, te **t) {
    switch (an->d[2].u4) {
        case AST_CLS(R):
        case AST_CLS(I):
        case AST_CLS(L):
        case AST_CLS(_):
            return AST_STAT(INV);
        case AST_CLS(E):
            *t = ((te*) an->d[3].p)->d[2].p;
            break;
        default:
            *t = an->d[3].p;
            break;
    }
    return *t ? AST_STAT(OK) : AST_STAT(INV);
}

static void t_r_f(void *p) {
    te *n = p;
    te_f(n->d[1].p); // free psr
    tbl_f(n->d[3].p); // alias/export tbl
    te_f(n->d[4].p);
    n->af->f(n);
}

static void t_te_f(void *p) {
    te *n = p;
    te_f(n->d[3].p);
    n->af->f(n);
}

static void t_i_f(void *p) {
    te *n = p;
    mc_f(n->d[3].p);
    n->af->f(n);
}

void ast_s_f(void *p) {
    te *n = p;
    te_f(n->d[3].p);
    n->af->f(n);
}

te *ast_s_i(ast *a, te *restrict pan, te *restrict psr, frfn nf, un ct, un v) {
    te *an = te_i(AST_MIN_LEN + 2, a->ta, nf);
    an->d[0] = P(pan);
    an->d[1] = P(psr);
    an->d[2] = U6(AST_CLS(S));
    an->d[3] = ct;
    an->d[4] = v;
    return an;
}

void ast_s_cs_f(void *p) {
    te *n = p;
    te_f(n->d[3].p);
    mc_f(n->d[4].p);
    n->af->f(n);
}

static void t_vl_f(void *p) {
    te *n = p;
    te_f(n->d[3].p);
    lst_f(n->d[4].p);
    n->af->f(n);
}

static void t_o_f(void *p) {
    te *n = p;
    te_f(n->d[3].p);
    te_f(n->d[5].p);
    te_f(n->d[6].p);
    n->af->f(n);
}

static void t_z_f(void *p) {
    te *n = p;
    te_f(n->d[3].p);
    te_f(n->d[4].p);
    mc_f(n->d[5].p);
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

static void t_c_f(void *p) {
    te *n = p;
    te_f(n->d[4].p);
    n->af->f(n);
}

te *ast_an_i(ast *a, te *restrict pan, te *restrict psr, ast_cls cls, un ct, ...) {
    size_t len = AST_MIN_LEN + 1; // add extra bucket at end for any
    frfn *nf = NULL;
    switch (cls) {
        case AST_CLS(R):
            len += 2;
            nf = t_r_f;
            break;
        case AST_CLS(T):
        case AST_CLS(E):
            nf = t_te_f;
            break;
        case AST_CLS(I):
            nf = t_i_f;
            break;
        case AST_CLS(S):
            len += 1;
            nf = ast_s_f;
            break;
        case AST_CLS(V):
            len += 1;
            nf = t_vl_f;
            break;
        case AST_CLS(O):
            len += 3;
            nf = t_o_f;
            break;
        case AST_CLS(Z):
            len += 2;
            nf = t_z_f;
            break;
        case AST_CLS(A):
            len += 2;
            nf = t_a_f;
            break;
        case AST_CLS(L):
            len += 1;
            nf = t_l_f;
            break;
        case AST_CLS(C):
            len += 1;
            nf = t_c_f;
            break;
        default:
            return NULL;
    }
    te *an = te_i(len, a->ta, nf);
    an->d[0] = P(pan);
    an->d[1] = P(psr);
    an->d[2] = U6(cls);
    an->d[3] = ct;
    va_list args;
    va_start(args, ct);
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

ast_stat ast_n(ast *a, te *restrict pan, te *restrict pn, void **vn, err **e) {
    ast_stat stat;
    size_t pid;
    if ((stat = a->pig(pn, &pid)) != AST_STAT(OK)) return stat;
    te *kv;
    if (tbl_g_i(a->pt, U6(pid), &kv) == TBL_STAT(NF)) {
        *e = err_i(a->ea, a->edp, (void*) te_f, te_c(pn), "ast_n");
        return AST_STAT(INV);
    }
    ast_tf *atf = kv->d[1].p;
    return atf(a, pan, pn, vn, e);
}

ast_stat ast_t_a(ast *a, uint16_t tid, uint16_t id) {
    te *kv;
    if (tbl_g_i(a->tt, U6(tid), &kv) == TBL_STAT(OK)) return AST_STAT(INV);
    kv = te_i(2, a->ta, NULL);
    kv->d[0] = U4(tid);
    kv->d[1] = U4(id);
    tbl_a(a->tt, kv);
    return AST_STAT(OK);
}

ast_stat ast_t_n(ast *a, te *t, uint16_t *id) {
    te *kv;
    if (tbl_g_i(a->tt, U4(tkn_m_g_i(t)), &kv) == TBL_STAT(NF)) return AST_STAT(INV);
    *id = kv->d[1].u4;
    return AST_STAT(OK);
}

void ast_f(ast *a) {
    if (!a || --a->r > 0) return;
    tbl_f(a->pt);
    tbl_f(a->tt);
    a->af->f(a);
}
