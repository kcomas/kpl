
#include "chk.h"

static void chk_p_conts(const tbl *ct, ast_cls base, size_t n, size_t idnt) {
    te *h = ct->i->h;
    while (h) {
        putchar('\n');
        for (size_t i = 0; i < idnt; i++) putchar(' ');
        te *kv = h->d[0].p;
        uint16_t cls = u4_g_o(kv->d[0], CHK_HSH_C);
        uint16_t type = u4_g_o(kv->d[0], CHK_HSH_T);
        if (n == 2 && base == AST_CLS(O)) printf("[%s %s]", ast_oc_str(cls), type_str(type));
        else printf("[%s %s]", ast_cls_str(cls), type_str(type));
        if (n > 0) chk_p_conts(kv->d[1].p, cls, n - 1, idnt + 1);
        h = h->d[2].p;
    }
}

void chk_p(const tbl *ct, size_t idnt) {
    te *h = ct->i->h;
    while (h) {
        te *kv = h->d[0].p;
        uint16_t cls = u4_g_o(kv->d[0], CHK_HSH_C);
        uint16_t type = u4_g_o(kv->d[0], CHK_HSH_T);
        printf("([%s %s]", ast_cls_str(cls), type_str(type));
        chk_p_conts(kv->d[1].p, cls, chk_cls_conts[cls] - 1, idnt + 1);
        printf(")\n");
        h = h->d[2].p;
    }
}

static chk_stat chk_cst_fn_lst_b(chk *c, te *an, te **e) {
    (void) c;
    (void) e;
    tbl *fat = ((te*) ((te*) an->d[5].p)->d[3].p)->d[3].p; // fn args tbl
    tbl *lt = ((te*) an->d[6].p)->d[3].p;
    te *h = lt->i->h, *lei, *kv;
    while (h) {
        lei = h->d[0].p;
        if (tbl_g_i(fat, lei->d[0], &kv) == TBL_STAT(OK)) {
            lei->d[2] = P(te_c(kv->d[2].p));
            // TODO set arg flag
        }
        h = h->d[2].p;
    }
    return CHK_STAT(OK);
}

static chk_stat chk_nop(chk *c, te *an, te **e) {
    (void) c;
    (void) an;
    (void) e;
    return CHK_STAT(OK);
}

static chk_stat chk_vd(chk *c, te *an, te **e) {
    (void) e;
    an->d[3] = P(type_s_i(c->a->ta, NULL, TYPE(VD)));
    return CHK_STAT(OK);
}

static chk_stat chk_aply_e_fn(chk *c, te *an, te **e) {
    (void) c;
    te *t = ((te*) ((te*) an->d[4].p)->d[3].p)->d[2].p;
    tbl *fa = t->d[3].p;
    lst *l = an->d[5].p;
    if (fa->i->l != l->l) { // args len
        *e = an;
        return CHK_STAT(INV);
    }
    te *fh = fa->i->h, *lh = l->h, *ft, *lt;
    while (fh && lh) {
        ft = ((te*) fh->d[0].p)->d[2].p;
        if (ast_g_t(lh->d[0].p, &lt) != AST_STAT(OK)) {
            *e = an;
            return CHK_STAT(INV);
        }
        if (!type_eq(ft, lt)) {
            *e = an;
            return CHK_STAT(INV);
        }
        fh = fh->d[2].p;
        lh = lh->d[2].p;
    }
    an->d[3] = P(te_c(t->d[2].p));
    return CHK_STAT(OK);
}

static chk_stat chk_dfn_e_op(chk *c, te *an, te **e) {
    (void) c;
    (void) e;
    an->d[3] = P(te_c(((te*) an->d[6].p)->d[3].p));
    ((te*) ((te*) an->d[5].p)->d[3].p)->d[2] = P(te_c(an->d[3].p));
    // TODO set local flag on entry
    return CHK_STAT(OK);
}

static chk_stat chk_cst_fn_lst(chk *c, te *an, te **e) {
    (void) c;
    (void) e;
    an->d[3] = P(te_c(((te*) an->d[5].p)->d[3].p));
    return CHK_STAT(OK);
}

static chk_stat chk_op_lr_teq(chk *c, te *an, te **e) {
    (void) c;
    te *lt, *rt;
    if (ast_g_t(an->d[5].p, &lt) != AST_STAT(OK)) {
        *e = an->d[5].p;
        return CHK_STAT(INV);
    }
    if (ast_g_t(an->d[6].p, &rt) != AST_STAT(OK)) {
        *e = an->d[6].p;
        return CHK_STAT(INV);
    }
    if (!type_eq(lt, rt)) {
        *e = an;
        return CHK_STAT(INV);
    }
    an->d[3] = P(te_c(lt));
    return CHK_STAT(OK);
}

static chk_stat chk_op_mon(chk *c, te *an, te **e) {
    (void) c;
    te *t;
    if (ast_g_t(an->d[6].p, &t) != AST_STAT(OK)) {
        *e = an;
        return CHK_STAT(INV);
    }
    an->d[3] = P(te_c(t));
    return CHK_STAT(OK);
}

chk *chk_b(chk *c) {
    // before
    CHK_AB(c, chk_cst_fn_lst_b, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(FN), AST_CLS(L), TYPE(_A));
    // after
    CHK_AA(c, chk_nop, AST_CLS(R), TYPE(_A), AST_CLS(A), TYPE(VD));
    CHK_AA(c, chk_nop, AST_CLS(S), TYPE(U5), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(S), TYPE(I6), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_vd, AST_CLS(A), TYPE(_N), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_aply_e_fn, AST_CLS(A), TYPE(_N), AST_CLS(E), TYPE(FN));
    // ops
    CHK_AA(c, chk_dfn_e_op, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(O), TYPE(FN));
    CHK_AA(c, chk_cst_fn_lst, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(FN), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_mon, AST_CLS(O), TYPE(_N), OC(SUB), TYPE(_A), AST_CLS(_), TYPE(_N), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(VD), OC(DUMP), TYPE(_A), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(I6));
    return c;
}
