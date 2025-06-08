
#include "chk.h"

static void chk_p_conts(const tbl *ct, ast_cls base, size_t n, size_t idnt) {
    te *h = ct->i->h;
    while (h) {
        putchar('\n');
        for (size_t i = 0; i < idnt; i++) putchar(' ');
        te *kv = h->d[0].p;
        uint16_t cls = u4_g_o(kv->d[0], AST_HSH_C);
        uint16_t type = u4_g_o(kv->d[0], AST_HSH_T);
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
        uint16_t cls = u4_g_o(kv->d[0], AST_HSH_C);
        uint16_t type = u4_g_o(kv->d[0], AST_HSH_T);
        printf("([%s %s]", ast_cls_str(cls), type_str(type));
        chk_p_conts(kv->d[1].p, cls, chk_cls_conts[cls] - 1, idnt + 1);
        printf(")\n");
        h = h->d[2].p;
    }
}

static chk_stat chk_err(chk *c, te *an, err **e, const char *m) {
    *e = err_i(c->ea, ast_err_p, (void*) te_f, te_c(an), m);
    return CHK_STAT(INV);
}

static void chk_lte_s_i(te *lte, uint32_t *r, uint32_t *x) {
    switch (((te*) lte->d[2].p)->d[1].u4) {
        case TYPE(F5):
        case TYPE(F6):
            ast_lst_tbl_e_s_i(lte, (*x)++);
            break;
        case TYPE(FN):
            ast_lst_tbl_e_s_f(lte, LTE_FLG(F));
            break;
        default:
            ast_lst_tbl_e_s_i(lte, (*r)++);
            break;
    }
}

static chk_stat chk_cst_fn_lst_b(chk *c, te *an, err **e) {
    (void) c;
    (void) e;
    tbl **fat = (tbl**) &((te*) ((te*) an->d[5].p)->d[3].p)->d[3].p; // fn args tbl
    tbl *lt = ((te*) an->d[6].p)->d[3].p;
    te *h, *lte, *kv;
    if (!*fat) {
        *fat = tbl_i_tbl(lt);
        h = lt->i->h;
        uint32_t ra = 0, xa = 0;
        while (h) {
            lte = h->d[0].p;
            if (!ast_lst_tbl_e_g_f(lte)) {
                if (!lte->d[2].p) return chk_err(c, an, e, "chk fn infer arg type not set");
                ast_lst_tbl_e_s_f(lte, LTE_FLG(A));
                chk_lte_s_i(lte, &ra, &xa);
                type_tbl_a(*fat, c->a->ta, mc_c(lte->d[0].p), ast_lst_tbl_e_g_i(lte), te_c(lte->d[2].p));
            }
            h = h->d[2].p;
        }
    } else {
        h = lt->i->h;
        while (h) {
            lte = h->d[0].p;
            if (tbl_g_i(*fat, lte->d[0], &kv) == TBL_STAT(OK)) {
                if (lte->d[2].p && !type_eq(kv->d[2].p, lte->d[2].p)) return chk_err(c, an, e, "chk fn type neq");
                lte->d[2] = P(te_c(kv->d[2].p));
                ast_lst_tbl_e_s_f(lte, LTE_FLG(A));
                ast_lst_tbl_e_s_i(lte, kv->d[1].u5);
            }
            h = h->d[2].p;
        }
    }
    te *p = an->d[0].p;
    if (p->d[2].u4 == AST_CLS(O) && p->d[4].u4 == OC(DFN) && p->d[5].p && ((te*) p->d[5].p)->d[2].u4 == AST_CLS(E)) { // set var so functions can be used before define
        lte = ((te*) p->d[5].p)->d[3].p;
        lte->d[2] = P(te_c(((te*) an->d[5].p)->d[3].p));
        p->d[3] = P(te_c(lte->d[2].p));
        ast_lst_tbl_e_s_f(lte, LTE_FLG(F));
    }
    return CHK_STAT(OK);
}

te *chk_g_pn_lte(te *an, const mc *s) {
    te *plns = an, *pln, *kv;
    do {
        pln = NULL;
        if (ast_g_pn(AST_CLS(L), plns, &pln) != AST_STAT(OK)) return NULL;
        plns = pln->d[0].p;
        if (tbl_g_i(pln->d[3].p, P(s), &kv) == TBL_STAT(OK)) return kv;
    } while (pln);
    return NULL;
}

static chk_stat chk_op_lst_b(chk *c, te *an, err **e) {
    (void) c;
    te *h = ((tbl*) ((te*) an->d[6].p)->d[3].p)->i->h, *lte, *kv;
    while (h) {
        lte = h->d[0].p;
        if (!lte->d[2].p && (kv = chk_g_pn_lte(an, lte->d[0].p))) {
            ast_lst_tbl_e_s_f(lte, LTE_FLG(O));
            lte->d[2] = P(te_c(kv->d[2].p));
        }
        if (!lte->d[2].p) return chk_err(c, an, e, "chk lst var not resolved");
        h = h->d[2].p;
    }
    return CHK_STAT(OK);
}

static chk_stat chk_nop(chk *c, te *an, err **e) {
    (void) c;
    (void) an;
    (void) e;
    return CHK_STAT(OK);
}

static chk_stat chk_vd(chk *c, te *an, err **e) {
    (void) e;
    an->d[3] = P(type_s_i(c->a->ta, NULL, TYPE(VD)));
    return CHK_STAT(OK);
}

static chk_stat chk_lst_l(chk *c, te *an, err **e) {
    te *h = ((tbl*) ((te*) an->d[4].p)->d[3].p)->i->h, *lte;
    uint32_t r = 0, x = 0;
    while (h) {
        lte = h->d[0].p;
        if (!lte->d[2].p) return chk_err(c, an, e, "chk lst var type inv");
        if (ast_lst_tbl_e_g_f(lte) & LTE_FLG(L)) chk_lte_s_i(lte, &r, &x);
        h = h->d[2].p;
    }
    return chk_vd(c, an, e);
}

static chk_stat chk_aply_e_fn(chk *c, te *an, err **e) {
    (void) c;
    // TODO check if FN without def
    te *t = ((te*) ((te*) an->d[4].p)->d[3].p)->d[2].p;
    tbl *fa = t->d[3].p;
    lst *l = an->d[5].p;
    if (fa->i->l != l->l) return chk_err(c, an, e, "chk args len");
    te *fh = fa->i->h, *lh = l->h, *ft, *lt;
    while (fh && lh) {
        ft = ((te*) fh->d[0].p)->d[2].p;
        if (ast_g_t(lh->d[0].p, &lt) != AST_STAT(OK)) return chk_err(c, an, e, "chk cannot get type");
        if (!type_eq(ft, lt)) return chk_err(c, an, e, "chk type ne");
        fh = fh->d[2].p;
        lh = lh->d[2].p;
    }
    an->d[3] = P(te_c(t->d[2].p));
    return CHK_STAT(OK);
}

static chk_stat chk_dfn_e_op(chk *c, te *an, err **e) {
    (void) c;
    te *lte = ((te*) an->d[5].p)->d[3].p, *r = an->d[6].p;
    if (lte->d[2].p != NULL || lte->d[3].p != NULL) return chk_err(c, an, e, "chk lte defined");
    an->d[3] = P(te_c(r->d[3].p));
    lte->d[2] = P(te_c(r->d[3].p));
    ast_lst_tbl_e_s_f(lte, LTE_FLG(L));
    return CHK_STAT(OK);
}

static chk_stat chk_cst_fn_lst(chk *c, te *an, err **e) {
    (void) c;
    tbl *lt = ((te*) an->d[6].p)->d[3].p;
    te *h = lt->i->h;
    while (h) {
        uint32_t flgs = ast_lst_tbl_e_g_f(h->d[0].p);
        if ((flgs & LTE_FLG(O)) && (!(flgs & LTE_FLG(F)))) return chk_err(c, an, e, "chk out of scope");
        h = h->d[2].p;
    }
    an->d[3] = P(te_c(((te*) an->d[5].p)->d[3].p));
    te *rt;
    if (ast_g_t(((lst*) ((te*) an->d[6].p)->d[4].p)->t->d[0].p, &rt) != AST_STAT(OK)) return chk_err(c, an, e, "chk cannot get return type");
    te **fr = (te**) &((te*) an->d[3].p)->d[2].p;
    if (!*fr) *fr = te_c(rt);
    else if (!type_eq(*fr, rt)) return chk_err(c, an, e, "chk lst stmt inv ret type");
    return CHK_STAT(OK);
}

static chk_stat chk_set_ret_op_l(chk *c, te *an, err **e) {
    te *lt;
    if (ast_g_t(an->d[5].p, &lt) != AST_STAT(OK)) return chk_err(c, an, e, "chk inv l type");
    an->d[3] = P(te_c(lt));
    return CHK_STAT(OK);
}

static chk_stat chk_op_e_n_s_dfn(chk *c, te *an, err **e) {
    (void) c;
    te *lte = ((te*) an->d[5].p)->d[3].p, *rt;
    if (ast_g_t(an->d[6].p, &rt) != AST_STAT(OK)) return chk_err(c, an->d[6].p, e, "chk cannot get op r type");
    lte->d[2] = P(te_c(rt));
    ast_lst_tbl_e_s_f(lte, LTE_FLG(L));
    an->d[3] = P(te_c(rt));
    return CHK_STAT(OK);
}

static chk_stat chk_op_lr(chk *c, te *an, err **e, te **lt) {
    (void) c;
    te *rt;
    if (ast_g_t(an->d[5].p, lt) != AST_STAT(OK)) return chk_err(c, an->d[5].p, e, "chk cannot get op l type");
    if (ast_g_t(an->d[6].p, &rt) != AST_STAT(OK)) return chk_err(c, an->d[6].p, e, "chk cannot get op r type");
    if (!type_eq(*lt, rt)) return chk_err(c, an, e, "chk op l r type ne");
    return CHK_STAT(OK);
}

static chk_stat chk_op_lr_teq_bl(chk *c, te *an, err **e) {
    chk_stat stat;
    te *lt;
    if ((stat = chk_op_lr(c, an, e, &lt)) != CHK_STAT(OK)) return stat;
    an->d[3] = P(type_s_i(c->a->ta, NULL, TYPE(BL)));
    return CHK_STAT(OK);
}

static chk_stat chk_op_lr_teq(chk *c, te *an, err **e) {
    chk_stat stat;
    te *lt;
    if ((stat = chk_op_lr(c, an, e, &lt)) != CHK_STAT(OK)) return stat;
    an->d[3] = P(te_c(lt));
    return CHK_STAT(OK);
}

static chk_stat chk_op_mon(chk *c, te *an, err **e) {
    (void) c;
    te *t;
    if (ast_g_t(an->d[6].p, &t) != AST_STAT(OK)) return chk_err(c, an, e, "chk cannot get type");
    an->d[3] = P(te_c(t));
    return CHK_STAT(OK);
}

chk *chk_b(chk *c) {
    // before
    CHK_AB(c, chk_cst_fn_lst_b, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(FN), AST_CLS(L), TYPE(_A));
    CHK_AB(c, chk_op_lst_b, AST_CLS(O), TYPE(_N), OC(LOOP), TYPE(_A), AST_CLS(O), TYPE(_N), AST_CLS(L), TYPE(_A));
    // after
    CHK_AA(c, chk_nop, AST_CLS(R), TYPE(_A), AST_CLS(A), TYPE(VD));
    CHK_AA(c, chk_nop, AST_CLS(S), TYPE(U5), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(S), TYPE(I6), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(S), TYPE(F6), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_lst_l, AST_CLS(A), TYPE(_N), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_aply_e_fn, AST_CLS(A), TYPE(_N), AST_CLS(E), TYPE(FN));
    // ops
    CHK_AA(c, chk_op_e_n_s_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(FN), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(FN), AST_CLS(O), TYPE(FN));
    CHK_AA(c, chk_dfn_e_op, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(O), TYPE(FN));
    CHK_AA(c, chk_cst_fn_lst, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(FN), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_set_ret_op_l, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(F6), AST_CLS(E), TYPE(U6));
    CHK_AA(c, chk_set_ret_op_l, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(U6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_vd, AST_CLS(O), TYPE(_N), OC(LOOP), TYPE(_A), AST_CLS(O), TYPE(BL), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_op_mon, AST_CLS(O), TYPE(_N), OC(SUB), TYPE(_A), AST_CLS(_), TYPE(_N), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(SUBA), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MUL), TYPE(_A), AST_CLS(E), TYPE(F6), AST_CLS(E), TYPE(F6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MUL), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MULA), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(DIV), TYPE(_A), AST_CLS(O), TYPE(F6), AST_CLS(O), TYPE(F6));
    CHK_AA(c, chk_op_lr_teq_bl, AST_CLS(O), TYPE(_N), OC(GT), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(VD), OC(DUMP), TYPE(_A), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(I6));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(VD), OC(DUMP), TYPE(_A), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(F6));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(VD), OC(DUMP), TYPE(_A), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(I6));
    return c;
}
