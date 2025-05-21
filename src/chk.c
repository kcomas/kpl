
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

chk_stat chk_err(const chk *c, te *an, err **e, const char *m) {
    te_c(ast_g_root(an));
    *e = err_i(c->ea, ast_err_p, ast_err_f, an, m);
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
        ast_lst_tbl_e_s_i(lte, c->fnlc--);
    }
    return CHK_STAT(OK);
}

static chk_stat chk_cst_nf_lst_b(chk *c, te *an, err **e) {
    chk_stat stat;
    if ((stat = chk_cst_fn_lst_b(c, an, e)) != CHK_STAT(OK)) return stat;
    te *h = ((tbl*) ((te*) an->d[6].p)->d[3].p)->i->h, *lte, *kv, *tc;
    tbl *fst = tbl_i_tbl(((te*) ((te*) an->d[5].p)->d[3].p)->d[3].p);
    uint32_t si = 0;
    while (h) {
        lte = h->d[0].p;
        if (lte->d[2].p) {
            h = h->d[2].p;
            continue;
        }
        if (!(kv = chk_g_pn_lte(an, lte->d[0].p))) return chk_err(c, an, e, "ckk nf cannot find var");
        if (!kv->d[2].p) return chk_err(c, an, e, "chk nf var inv");
        ast_lst_tbl_e_s_i(lte, si++);
        ast_lst_tbl_e_s_f(lte, LTE_FLG(S));
        lte->d[2] = P(te_c(kv->d[2].p));
        h = h->d[2].p;
        tc = te_i_te(lte);
        tc->d[0] = P(mc_c(lte->d[0].p));
        tc->d[1] = U6(ast_lst_tbl_e_g_i(lte));
        tc->d[2] = P(te_c(lte->d[2].p));
        tbl_a(fst, tc);
    }
    ((te*) ((te*) an->d[5].p)->d[3].p)->d[4] = P(fst);
    return CHK_STAT(OK);
}

te *chk_g_pn_lte(te *an, const mc *s) {
    te *plns = an, *pln, *kv;
    do {
        pln = NULL;
        if (ast_g_pn(AST_CLS(L), plns, &pln) != AST_STAT(OK)) return NULL;
        plns = pln->d[0].p;
        if (tbl_g_i(pln->d[3].p, P(s), &kv) == TBL_STAT(OK) && kv->d[2].p) return kv;
        // TODO don't continue past FN
    } while (pln);
    return NULL;
}

static chk_stat chk_op_lst_side(chk *c, te *an, err **e, size_t side, bool fc) {
    tbl *t = ((te*) an->d[side].p)->d[3].p;
    if (!t) return CHK_STAT(OK);
    te *h = t->i->h, *lte, *kv;
    while (h) {
        lte = h->d[0].p;
        if (!lte->d[2].p && (kv = chk_g_pn_lte(an, lte->d[0].p))) {
            if (!kv->d[2].p) return chk_err(c, an, e, "chk lst var type NULL");
            lte->d[1] = kv->d[1];
            ast_lst_tbl_e_s_f(lte, LTE_FLG(O));
            lte->d[2] = P(te_c(kv->d[2].p));
        }
        if (fc && !lte->d[2].p) return chk_err(c, an, e, "chk lst var type not resolved");
        h = h->d[2].p;
    }
    return CHK_STAT(OK);
}

static chk_stat chk_op_lst_lr_b(chk *c, te *an, err **e) {
    chk_stat cstat;
    if (an->d[0].p && ((te*) an->d[0].p)->d[2].u4 == AST_CLS(L) && (cstat = chk_op_lst_side(c, an, e, 0, false)) != CHK_STAT(OK)) return cstat; // ignore unresolved
    if ((cstat = chk_op_lst_side(c, an, e, 5, true)) != CHK_STAT(OK)) return cstat;
    return chk_op_lst_side(c, an, e, 6, true);
}

static chk_stat chk_vec(chk *c, te *an, err **e) {
    lst *l = an->d[4].p;
    an->d[3] = P(type_te_i(c->ta, NULL, l->l));
    size_t lp = 2;
    te *h = l->h, *lt;
    while (h) {
        if (ast_g_t(h->d[0].p, &lt) != AST_STAT(OK)) return chk_err(c, an, e, "chk inv type in vec");
        lt->d[0] = an->d[3];
        ((te*) an->d[3].p)->d[lp++] = P(te_c(lt));
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

static chk_stat chk_bl(chk *c, te *an, err **e) {
    (void) e;
    an->d[3] = P(type_s_i(c->a->ta, NULL, TYPE(BL)));
    return CHK_STAT(OK);
}

static chk_stat chk_l_lst_bl(chk *c, te *an, err **e) {
    te *ln = an->d[5].p;
    if (ln->d[2].u4 != AST_CLS(L)) return chk_err(c, an, e, "chk l not lst");
    ln = ((lst*) ln->d[4].p)->t->d[0].p;
    if (((te*) ln->d[3].p)->d[1].u4 != TYPE(BL)) return chk_err(c, an, e, "chk lst ret not bl");
    return chk_vd(c, an, e);
}

static chk_stat chk_lst_l(chk *c, te *an, err **e) {
    te *h = ((tbl*) ((te*) an->d[4].p)->d[3].p)->i->h, *lte;
    uint32_t r = 0, x = 0, flgs;
    while (h) {
        lte = h->d[0].p;
        if (!lte->d[2].p) return chk_err(c, an, e, "chk lst var type inv");
        flgs = ast_lst_tbl_e_g_f(lte);
        if (flgs & LTE_FLG(L)) chk_lte_s_i(lte, &r, &x);
        if (flgs & LTE_FLG(E) && !(flgs & LTE_FLG(O)) && (!an->d[0].p || ((te*) an->d[0].p)->d[2].u4 != AST_CLS(R))) return chk_err(c, an, e, "chk inv exp");
        h = h->d[2].p;
    }
    return chk_vd(c, an, e);
}

static chk_stat chk_if(chk *c, te *an, err **e) {
    lst *bl = ((te*) an->d[6].p)->d[4].p;
    te *h = bl->h, *rt, *tt, *pn;
    if (bl->l == 1) {
        pn = an->d[0].p;
        if (!pn || pn->d[2].u4 != AST_CLS(L)) return chk_vd(c, an, e);
        pn = pn->d[0].p;
        if (!pn || pn->d[2].u4 != AST_CLS(O) || pn->d[4].u4 != OC(IF)) return chk_vd(c, an, e);
        if (ast_g_t(h->d[0].p, &rt) != AST_STAT(OK)) return chk_err(c, an, e, "chk cannot get type");
        an->d[3] = P(te_c(rt));
        return CHK_STAT(OK);
    }
    if (ast_g_t(h->d[0].p, &rt) != AST_STAT(OK)) return chk_err(c, an, e, "chk cannot get type");
    h = h->d[2].p;
    while (h) {
        if (ast_g_t(h->d[0].p, &tt) != AST_STAT(OK)) return chk_err(c, an, e, "chk cannot get type");
        if (!type_eq(rt, tt)) return chk_err(c, an, e, "chk if lst type neq");
        h = h->d[2].p;
    }
    an->d[3] = P(te_c(rt));
    return CHK_STAT(OK);
}

static chk_stat chk_aply_e_fn(chk *c, te *an, err **e) {
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

static chk_stat te_g_rt(chk *c, te *an, err **e, te **rt) {
    lst *l = an->d[5].p;
    if (l->l != 1) return chk_err(c, an, e, "chk aply len inv");
    te *s = l->h->d[0].p, *lte;
    if (s->d[2].u4 != AST_CLS(S)) return chk_err(c, an, e, "chk aply not scalar");
    if (((te*) s->d[3].p)->d[1].u4 != TYPE(I6)) return chk_err(c, an, e, "chk aply inv scalar type");
    if (ast_g_t(an->d[4].p, &lte) != AST_STAT(OK)) return chk_err(c, an, e, "chk inv e type");
    *rt = type_te_g_t(lte, s->d[4].i6);
    if (!*rt) return chk_err(c, an, e, "chk aply te out of bounds");
    return CHK_STAT(OK);
}

static chk_stat chk_aply_e_te(chk *c, te *an, err **e) {
    chk_stat stat;
    te *rt;
    if ((stat = te_g_rt(c, an, e, &rt)) != CHK_STAT(OK)) return stat;
    an->d[3] = P(te_c(rt));
    return CHK_STAT(OK);
}

static chk_stat chk_cst_fn_lst(chk *c, te *an, err **e) {
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
    if (*fr && (*fr)->d[1].u4 == TYPE(VD)) return CHK_STAT(OK);
    else if (!*fr) *fr = te_c(rt);
    else if (!type_eq(*fr, rt)) return chk_err(c, an, e, "chk lst stmt inv ret type");
    return CHK_STAT(OK);
}

static chk_stat chk_set_ret_op_l(chk *c, te *an, err **e) {
    te *lt;
    if (ast_g_t(an->d[5].p, &lt) != AST_STAT(OK)) return chk_err(c, an, e, "chk inv l type");
    an->d[3] = P(te_c(lt));
    return CHK_STAT(OK);
}

static chk_stat chk_cst_vr_v(chk *c, te *an, err **e) {
    te *lt = ((te*) an->d[5].p)->d[3].p, *rt = ((te*) an->d[6].p)->d[3].p;
    if (!lt->d[2].p) {
        if (!type_te_eq(rt)) return chk_err(c, an, e, "chk te type inv for vr");
        lt->d[2] = P(type_cpy(rt->d[2].p));
        ((te*) lt->d[2].p)->d[0] = P(lt);
    }
    if (!lt->d[2].p) return chk_err(c, an, e, "chk inv vr type not set");
    an->d[3] = P(te_c(lt));
    return CHK_STAT(OK);
}

static chk_stat chk_op_e_n_svo_dfn(chk *c, te *an, err **e) {
    te *lte = ((te*) an->d[5].p)->d[3].p, *rt;
    if (ast_g_t(an->d[6].p, &rt) != AST_STAT(OK)) return chk_err(c, an->d[6].p, e, "chk cannot get op r type");
    lte->d[2] = P(te_c(rt));
    ast_lst_tbl_e_s_f(lte, LTE_FLG(L));
    an->d[3] = P(te_c(rt));
    return CHK_STAT(OK);
}

static chk_stat chk_agn_a_o(chk *c, te *an, err **e) {
    chk_stat stat;
    te *lt, *rt;
    if ((stat = te_g_rt(c, an->d[5].p, e, &lt)) != CHK_STAT(OK)) return stat;
    if (ast_g_t(an->d[6].p, &rt) != AST_STAT(OK)) return chk_err(c, an, e, "chk agn rt inv");
    if (!type_eq(lt, rt)) return chk_err(c, an, e, "chk agn type neq");
    an->d[3] = P(te_c(rt));
    return CHK_STAT(OK);
}

static chk_stat chk_op_lr(chk *c, te *an, err **e, te **lt) {
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
    return chk_bl(c, an, e);
}

chk_stat chk_op_lr_teq(chk *c, te *an, err **e) {
    chk_stat stat;
    te *lt;
    if ((stat = chk_op_lr(c, an, e, &lt)) != CHK_STAT(OK)) return stat;
    an->d[3] = P(te_c(lt));
    return CHK_STAT(OK);
}

chk_stat chk_op_lers_us_u(chk *c, te *an, err **e) {
    (void) c;
    (void) e;
    te *r = an->d[6].p;
    ((te*) r->d[3].p)->d[1] = U4(TYPE(U6));
    an->d[3] = P(te_c(r->d[3].p));
    return CHK_STAT(OK);
}

chk_stat chk_op_lers_fs_f(chk *c, te *an, err **e) {
    (void) c;
    (void) e;
    te *r = an->d[6].p;
    ((te*) r->d[3].p)->d[1] = U4(TYPE(F6));
    r->d[4] = F6((double) r->d[4].i6);
    an->d[3] = P(te_c(r->d[3].p));
    return CHK_STAT(OK);
}

static chk_stat chk_op_lers_us_bl(chk *c, te *an, err **e) {
    te *r = an->d[6].p;
    ((te*) r->d[3].p)->d[1] = U4(TYPE(U6));
    return chk_bl(c, an, e);
}

static chk_stat chk_op_lers_fs_bl(chk *c, te *an, err **e) {
    te *r = an->d[6].p;
    ((te*) r->d[3].p)->d[1] = U4(TYPE(F6));
    r->d[4] = F6((double) r->d[4].i6);
    return chk_bl(c, an, e);
}

chk_stat chk_op_mon(chk *c, te *an, err **e) {
    te *t;
    if (ast_g_t(an->d[6].p, &t) != AST_STAT(OK)) return chk_err(c, an, e, "chk cannot get type");
    an->d[3] = P(te_c(t));
    return CHK_STAT(OK);
}

static chk_stat chk_op_levrro_vr(chk *c, te *an, err **e) {
    te *lte = ((te*) an->d[5].p)->d[3].p, *rt = ((te*) an->d[6].p)->d[3].p;
    if (!type_eq(((te*) lte->d[2].p)->d[2].p, rt)) return chk_err(c, an, e, "chk l vr type and r type neq");
    an->d[3] = P(te_c(lte->d[2].p));
    return CHK_STAT(OK);
}

void chk_arith(chk *c);

chk *chk_b(chk *c) {
    // before
    CHK_AB(c, chk_cst_fn_lst_b, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(FN), AST_CLS(L), TYPE(_A));
    CHK_AB(c, chk_cst_nf_lst_b, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(NF), AST_CLS(L), TYPE(_A));
    CHK_AB(c, chk_op_lst_lr_b, AST_CLS(O), TYPE(_N), OC(LOOP), TYPE(_A), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A));
    CHK_AB(c, chk_op_lst_lr_b, AST_CLS(O), TYPE(_N), OC(IF), TYPE(_A), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A));
    // after
    CHK_AA(c, chk_nop, AST_CLS(R), TYPE(_A), AST_CLS(A), TYPE(VD));
    CHK_AA(c, chk_nop, AST_CLS(S), TYPE(U5), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(S), TYPE(I6), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(S), TYPE(F6), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_vec, AST_CLS(V), TYPE(_N), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_lst_l, AST_CLS(A), TYPE(_N), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_aply_e_fn, AST_CLS(A), TYPE(_N), AST_CLS(E), TYPE(FN));
    CHK_AA(c, chk_aply_e_fn, AST_CLS(A), TYPE(_N), AST_CLS(E), TYPE(NF));
    CHK_AA(c, chk_aply_e_te, AST_CLS(A), TYPE(_N), AST_CLS(E), TYPE(TE));
    // ops
    CHK_AA(c, chk_op_e_n_svo_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_e_n_svo_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(S), TYPE(F6));
    CHK_AA(c, chk_op_e_n_svo_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_e_n_svo_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(O), TYPE(U6));
    CHK_AA(c, chk_op_e_n_svo_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(O), TYPE(VR));
    CHK_AA(c, chk_op_e_n_svo_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(V), TYPE(TE));
    CHK_AA(c, chk_agn_a_o, AST_CLS(O), TYPE(_N), OC(AGN), TYPE(_A), AST_CLS(A), TYPE(I6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(FN), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(FN), AST_CLS(O), TYPE(FN));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(NF), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(NF), AST_CLS(O), TYPE(NF));
    CHK_AA(c, chk_cst_fn_lst, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(FN), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_cst_fn_lst, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(NF), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_set_ret_op_l, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(F6), AST_CLS(E), TYPE(U6));
    CHK_AA(c, chk_set_ret_op_l, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(U6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_cst_vr_v, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(VR), AST_CLS(V), TYPE(TE));
    CHK_AA(c, chk_l_lst_bl, AST_CLS(O), TYPE(_N), OC(LOOP), TYPE(_A), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_if, AST_CLS(O), TYPE(_N), OC(IF), TYPE(_A), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_op_lr_teq_bl, AST_CLS(O), TYPE(_N), OC(GT), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lers_us_bl, AST_CLS(O), TYPE(_N), OC(EQ), TYPE(_A), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lers_us_bl, AST_CLS(O), TYPE(_N), OC(GT), TYPE(_A), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq_bl, AST_CLS(O), TYPE(_N), OC(LT), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lers_fs_bl, AST_CLS(O), TYPE(_N), OC(LT), TYPE(_A), AST_CLS(E), TYPE(F6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq_bl, AST_CLS(O), TYPE(_N), OC(LTE), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq_bl, AST_CLS(O), TYPE(_N), OC(AND), TYPE(_A), AST_CLS(O), TYPE(BL), AST_CLS(O), TYPE(BL));
    CHK_AA(c, chk_op_levrro_vr, AST_CLS(O), TYPE(_N), OC(CNCTA), TYPE(_A), AST_CLS(E), TYPE(VR), AST_CLS(O), TYPE(F6));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(VD), OC(DUMP), TYPE(_A), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(I6));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(VD), OC(DUMP), TYPE(_A), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(F6));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(VD), OC(DUMP), TYPE(_A), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(U6));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(VD), OC(DUMP), TYPE(_A), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(VD), OC(DUMP), TYPE(_A), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(TE));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(VD), OC(DUMP), TYPE(_A), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(VR));
    chk_arith(c);
    return c;
}
