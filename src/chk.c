
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
    if (lt && !*fat) {
        *fat = tbl_i_tbl(lt);
        h = lt->i->h;
        uint32_t ra = 0, xa = 0;
        while (h) {
            lte = h->d[0].p;
            if (!ast_lst_tbl_e_g_f(lte) && lte->d[2].p) {
                ast_lst_tbl_e_s_f(lte, LTE_FLG(A));
                chk_lte_s_i(lte, &ra, &xa);
                type_tbl_a(*fat, c->a->ta, mc_c(lte->d[0].p), ast_lst_tbl_e_g_i(lte), te_c(lte->d[2].p));
            }
            h = h->d[2].p;
        }
    } else if (lt) {
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
        if (!lte->d[2].p) lte->d[2] = P(te_c(((te*) an->d[5].p)->d[3].p));
        p->d[3] = P(te_c(lte->d[2].p));
        ast_lst_tbl_e_s_f(lte, LTE_FLG(F));
        ast_lst_tbl_e_s_i(lte, c->fnlc--);
    }
    return CHK_STAT(OK);
}

static void *chk_g_pn_lte_(te *an, const mc *s, bool tbl) {
    bool pf = false; // past function
    te *plns = an, *pln, *kv;
    do {
        pln = NULL;
        if (ast_g_pn(AST_CLS(L), plns, &pln) != AST_STAT(OK)) return NULL;
        plns = pln->d[0].p;
        if (tbl_g_i(pln->d[3].p, P(s), &kv) == TBL_STAT(OK) && kv->d[2].p) {
            if (pf && ((te*) kv->d[2].p)->d[1].u4 != TYPE(FN)) return NULL;
            return tbl ? pln->d[3].p : kv;
        }
        if (plns->d[2].u4 == AST_CLS(O) && plns->d[4].u4 == OC(CST)) {
            if (ast_g_t(plns->d[5].p, &kv) != AST_STAT(OK)) return NULL;
            if (kv->d[1].u4 == TYPE(FN)) pf = true;
        }
    } while (pln);
    return NULL;
}

te *chk_g_pn_lte(te *an, const mc *s) {
   return (te*) chk_g_pn_lte_(an, s, false);
}

tbl *chk_g_pn_lte_tbl(te *an, const mc *s) {
   return (tbl*) chk_g_pn_lte_(an, s, true);
}

te *chk_g_pn_fnnf_type(te *an) {
    te *pn;
    while (ast_g_pn(AST_CLS(O), an->d[0].p, &pn) == AST_STAT(OK)) {
        an = pn;
        if (pn->d[4].u4 != OC(CST)) continue;
        pn = pn->d[5].p;
        if (pn->d[2].u4 != AST_CLS(T)) continue;
        pn = pn->d[3].p;
        if (type_g_c(pn->d[1].u4 == TYPE_CLS(F))) return pn;
    }
    return NULL;
}

te *un_g_lte_ch(te *un, char c) {
    if (!un) return NULL;
    te *h = ((tbl*) un->d[2].p)->i->h, *n;
    while (h) {
        n = h->d[0].p;
        mc *s = n->d[0].p;
        if (s->l >= 2 && s->d[0] == c && s->d[1] == '\0') return n;
        h = h->d[2].p;
    }
    return NULL;
}

chk_stat chk_fnnf_un_ret(chk *c, te *an, err **e) {
    tbl *ft;
    te *fn = chk_g_pn_fnnf_type(an), *fe, *ae;
    if (!fn) return CHK_STAT(OK);
    if (!fn->d[2].p) fn->d[2] = P(type_h_i(c->ta, NULL, TYPE(UN), tbl_i(c->tbla, tbl_mc_sdbm, tbl_mc_eq, lst_i(c->la, c->ta, (void*) te_f), te_i(4, c->ta, NULL))));
    fe = un_g_lte_ch(fn->d[2].p, 'e');
    ae = un_g_lte_ch(an->d[3].p, 'e');
    if (!ae) return chk_err(c, an, e, "chk inv un for access inside fn");
    if (!fe) {
        ft = ((te*) fn->d[2].p)->d[2].p;
        tbl_a(ft, te_c(ae));
        lst_s(ft->i, type_h_cmp);
    } else if (!type_eq(fe->d[2].p, ae->d[2].p)) return chk_err(c, an, e, "chk inv un fn e type for impl err");
    return CHK_STAT(OK);
}

static chk_stat chk_cst_nf_lst_b(chk *c, te *an, err **e) {
    chk_stat stat;
    if ((stat = chk_cst_fn_lst_b(c, an, e)) != CHK_STAT(OK)) return stat;
    tbl *nft = ((te*) an->d[6].p)->d[3].p, *fst = tbl_i_tbl(((te*) ((te*) an->d[5].p)->d[3].p)->d[3].p);
    te *h = nft->i->h, *lte, *kv, *tc;
    uint32_t si = 0;
    while (h) {
        lte = h->d[0].p;
        if (lte->d[2].p || !(kv = chk_g_pn_lte(an, lte->d[0].p))) {
            h = h->d[2].p;
            continue;
        }
        if (kv->d[2].p && type_g_c(((te*) kv->d[2].p)->d[1].u4) == TYPE_CLS(F)) {
            te_f(lte);
            h->d[0] = P(te_c(kv));
            h = h->d[2].p;
            continue;
        }
        ast_lst_tbl_e_s_i(lte, si++);
        ast_lst_tbl_e_s_f(lte, LTE_FLG(S));
        lte->d[2] = P(te_c(kv->d[2].p));
        tc = te_i_te(lte);
        tc->d[0] = P(mc_c(lte->d[0].p));
        tc->d[1] = U6(ast_lst_tbl_e_g_i(lte));
        tc->d[2] = P(te_c(lte->d[2].p));
        tbl_a(fst, tc);
        h = h->d[2].p;
    }
    ((te*) ((te*) an->d[5].p)->d[3].p)->d[4] = P(fst);
    return CHK_STAT(OK);
}

static chk_stat chk_op_lst_side(chk *c, te *an, err **e, size_t side) {
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
        h = h->d[2].p;
    }
    return CHK_STAT(OK);
}

static chk_stat chk_op_lst_lr_b(chk *c, te *an, err **e) {
    chk_stat cstat;
    if (an->d[0].p && ((te*) an->d[0].p)->d[2].u4 == AST_CLS(L) && (cstat = chk_op_lst_side(c, an, e, 0)) != CHK_STAT(OK)) return cstat; // ignore unresolved
    if ((cstat = chk_op_lst_side(c, an, e, 5)) != CHK_STAT(OK)) return cstat;
    return chk_op_lst_side(c, an, e, 6);
}

static chk_stat chk_op_mtch_lst_lr_b(chk *c, te *an, err **e) {
    if (chk_n(c, an->d[5].p, e) != CHK_STAT(OK)) return chk_err(c, an, e, "chk inv match right");
    lst *l = ((te*) an->d[5].p)->d[4].p;
    te *ut, *h, *n, *kve, *kvt;
    if (!l) return chk_op_lst_lr_b(c, an, e);
    if (ast_g_t(l->h->d[0].p, &ut) != AST_STAT(OK)) return chk_err(c, an, e, "chk cannot get mtch tgt type");
    h = ((lst*) ((te*) an->d[6].p)->d[4].p)->h;
    while (h) {
        n = h->d[0].p;
        if (n->d[2].u4 == AST_CLS(Z)) {
            if (tbl_g_i(((te*) an->d[6].p)->d[3].p, n->d[5], &kve) != TBL_STAT(OK)) return chk_err(c, an, e, "chk mtch inv sym var");
            if (kve->d[2].p) return chk_err(c, an, e, "chk mtch sym dup");
            if (tbl_g_i(ut->d[2].p, n->d[5], &kvt) != TBL_STAT(OK)) return chk_err(c, an, e, "chk inv un sym type entry");
            kve->d[2] = P(te_c(kvt->d[2].p));
            ast_lst_tbl_e_s_i(kve, c->yc--);
            ast_lst_tbl_e_s_f(kve, LTE_FLG(Y));
            ((te*) n->d[3].p)->d[2] = P(type_s_i(c->a->ta, NULL, TYPE(VD)));
        } else if (n->d[2].u4 == AST_CLS(A)) return chk_err(c, an, e, "nyi");
        else if (h->d[2].p) return chk_err(c, an, e, "chk inv mtch default case");
        h = h->d[2].p;
    }
    return CHK_STAT(OK);
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

static chk_stat chk_z_type_h(chk *c, te *an, err **e) {
    te *st, *kv;
    if (ast_g_t(an->d[4].p, &st) != AST_STAT(OK)) return chk_err(c, an, e, "chk st cannot get access type");
    // TODO determine if this is inside a function and if it returns error
    if (tbl_g_i(st->d[2].p, an->d[5], &kv) != TBL_STAT(OK)) return chk_err(c, an, e, "chk st inv access type");
    ((te*) an->d[3].p)->d[2] = P(te_c(kv->d[2].p));
    return CHK_STAT(OK);
}

static chk_stat chk_z_vd(chk *c, te *an, err **e) {
    (void) e;
    ((te*) an->d[3].p)->d[2] = P(type_s_i(c->a->ta, NULL, TYPE(VD)));
    return CHK_STAT(OK);
}

chk_stat chk_nop(chk *c, te *an, err **e) {
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
    tbl *lt = ((te*) an->d[4].p)->d[3].p;
    if (!lt) return chk_vd(c, an, e);
    te *h = lt->i->h, *lte;
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

static chk_stat chk_auto_cond(chk *c, te **an, err **e) {
    te *t;
    if ((*an)->d[2].u4 != AST_CLS(E)) return chk_err(c, *an, e, "chk auto cond inv ast cls");
    if (ast_g_t(*an, &t) != AST_STAT(OK)) return chk_err(c, *an, e, "chk auto inv type");
    switch (t->d[1].u4) {
        case TYPE(I6):
            *an = ast_an_i(c->a, (*an)->d[0].p, (*an)->d[1].p, AST_CLS(O), P(type_s_i(c->a->ta, NULL, TYPE(BL))), U4(OC(NE)), *an, ast_an_i(c->a, (*an)->d[0].p, (*an)->d[1].p, AST_CLS(S), P(type_s_i(c->a->ta, NULL, TYPE(I6))), U5(0)));
            break;
        default:
            return chk_err(c, *an, e, "chk inv type for auto cond");
    }
    return CHK_STAT(OK);
}

static te *or_lst_type(lst *l) {
    te *kv = NULL;
    if (!l) return NULL;
    if (l->l != 1) return NULL;
    if (ast_g_t(l->h->d[0].p, &kv) != AST_STAT(OK)) return NULL;
    return kv;
}

static chk_stat chk_and_or(chk *c, te *an, err **e) {
    chk_stat stat;
    te *l = an->d[5].p, *r = an->d[6].p;
    if (!(l = or_lst_type(l->d[4].p))) return chk_err(c, an, e, "chk or inv lst type");
    if (l->d[1].u4 != TYPE(BL)) {
        l = an->d[5].p;
        if ((stat = chk_auto_cond(c, (te**) &((lst*) l->d[4].p)->h->d[0].p, e)) != CHK_STAT(OK)) return stat;
    }
    if (!(r = or_lst_type(r->d[4].p))) return chk_err(c, an, e, "chk or inv lst type");
    if (r->d[1].u4 != TYPE(BL)) {
        r = an->d[6].p;
        if ((stat = chk_auto_cond(c, (te**) &((lst*) r->d[4].p)->h->d[0].p, e)) != CHK_STAT(OK)) return stat;
    }
    an->d[3] = P(type_s_i(c->a->ta, NULL, TYPE(BL)));
    return CHK_STAT(OK);
}

static chk_stat st_init(chk *c, te *an, err **e) {
    lst *l = ((te*) an->d[6].p)->d[4].p;
    if (!l) return chk_err(c, an, e, "chk st init inv lst");
    te *h = l->h, *z, *zt;
    tbl *tt = tbl_i(c->tbla, tbl_mc_sdbm, tbl_mc_eq, lst_i(c->la, c->ta, (void*) te_f), te_i(l->l * 2, c->ta, NULL));
    h = l->h;
    while (h) {
        z = h->d[0].p;
        if (z->d[2].u4 != AST_CLS(Z)) {
            tbl_f(tt);
            return chk_err(c, an, e, "chk st init inv lst itm");
        }
        if (ast_g_t(z->d[4].p, &zt) != AST_STAT(OK)) {
            tbl_f(tt);
            return chk_err(c, an, e, "chk st init inv z type");
        }
        type_tbl_a(tt, c->ta, mc_c(z->d[5].p), 0, te_c(zt));
        h = h->d[2].p;
    }
    lst_s(tt->i, type_h_cmp);
    an->d[3] = P(type_h_i(c->ta, NULL, TYPE(ST), tt));
    return CHK_STAT(OK);
}

static chk_stat chk_mtch(chk *c, te *an, err **e) {
    if (!((te*) an->d[5].p)->d[4].p) return st_init(c, an, e);
    te *pn = an->d[0].p, *t, *rt = NULL, *h, *n;
    if (pn->d[2].u4 == AST_CLS(L) && (((te*) pn->d[0].p)->d[2].u4 == AST_CLS(R) || ((lst*) pn->d[4].p)->t->d[0].p != an)) return CHK_STAT(OK);
    // TODO chk for parent op or return else set return type to void
    ssize_t mi = 0;
    h = ((lst*) ((te*) an->d[6].p)->d[4].p)->h;
    while (h) {
        n = h->d[0].p;
        if (n->d[2].u4 == AST_CLS(Z) || n->d[2].u4 == AST_CLS(A)) {
            n = n->d[4].p;
            mi++;
        } else mi = -1;
        if (ast_g_t(n, &t) != AST_STAT(OK)) return chk_err(c, an, e, "chk inv mtch type");
        if (rt && !type_eq(rt, t)) return chk_err(c, an, e, "chk inv mtch type neq");
        else rt = t;
        h = h->d[2].p;
    }
    if (rt->d[1].u4 != TYPE(VD) && mi > -1 && mi < (ssize_t) ((lst*) ((te*) an->d[6].p)->d[4].p)->l) return chk_err(c, an, e, "chk mtch need def case");
    an->d[3] = P(te_c(rt));
    return CHK_STAT(OK);
}

static chk_stat chk_fn_args(chk *c, te *restrict an, err **e, te *restrict t, lst *l) {
    tbl *fa = t->d[3].p;
    if (!l || fa->i->l == 0) return CHK_STAT(OK);
    if (fa->i->l != l->l) return chk_err(c, an, e, "chk args len");
    te *fh = fa->i->h, *lh = l->h, *ft, *lt;
    while (fh && lh) {
        ft = ((te*) fh->d[0].p)->d[2].p;
        if (ast_g_t(lh->d[0].p, &lt) != AST_STAT(OK)) return chk_err(c, an, e, "chk cannot get type");
        if (!type_eq(ft, lt)) return chk_err(c, an, e, "chk type ne");
        fh = fh->d[2].p;
        lh = lh->d[2].p;
    }
    return CHK_STAT(OK);
}

static chk_stat chk_aply_fn(chk *c, te *restrict an, err **e, te *restrict t, lst *l) {
    chk_stat stat;
    if ((stat = chk_fn_args(c, an, e, t, l)) != CHK_STAT(OK)) return stat;
    an->d[3] = P(te_c(t->d[2].p));
    return CHK_STAT(OK);
}

static chk_stat chk_aply_e_fn(chk *c, te *an, err **e) {
    te *t = ((te*) ((te*) an->d[4].p)->d[3].p)->d[2].p;
    return chk_aply_fn(c, an, e, t, an->d[5].p);
}

static chk_stat chk_aply_s_fn(chk *c, te *an, err **e) {
    te *t = ((te*) an->d[4].p)->d[3].p;
    return chk_aply_fn(c, an, e, t, an->d[5].p);
}

chk_stat a_te_g_rt(chk *c, te *an, err **e, te **rt) {
    lst *l = an->d[5].p;
    if (l->l != 1) return chk_err(c, an, e, "chk aply len inv");
    te *s = l->h->d[0].p, *tn;
    if (s->d[2].u4 != AST_CLS(S)) return chk_err(c, an, e, "chk aply not scalar");
    if (((te*) s->d[3].p)->d[1].u4 != TYPE(I6)) return chk_err(c, an, e, "chk aply inv scalar type");
    if (ast_g_t(an->d[4].p, &tn) != AST_STAT(OK)) return chk_err(c, an, e, "chk inv e type");
    if (tn->d[1].u4 == TYPE(SL)) tn = tn->d[2].p;
    *rt = type_te_g_t(tn, s->d[4].i6);
    if (!*rt) return chk_err(c, an, e, "chk aply te out of bounds");
    return CHK_STAT(OK);
}

static chk_stat chk_aply_ez_te(chk *c, te *an, err **e) {
    chk_stat stat;
    te *rt;
    if ((stat = a_te_g_rt(c, an, e, &rt)) != CHK_STAT(OK)) return stat;
    an->d[3] = P(te_c(rt));
    return CHK_STAT(OK);
}

static chk_stat chk_aply_z_fn(chk *c, te *an, err **e) {
    chk_stat stat;
    te *t;
    if (ast_g_t(an->d[4].p, &t) != AST_STAT(OK)) return chk_err(c, an, e, "chk aply z cannot get z type");
    t = t->d[2].p;
    if ((stat = chk_fn_args(c, an, e, t, an->d[5].p)) != CHK_STAT(OK)) return stat;
    an->d[3] = P(te_c(t->d[2].p));
    return CHK_STAT(OK);
}

static const char *aply_cs_str[TYPE(_END)] = {
    [TYPE(I6)] = "%ld",
    [TYPE(F6)] = "%lf"
};

static chk_stat chk_aply_cs(chk *c, te *an, err **e) {
    mc *s = ((te*) an->d[4].p)->d[4].p;
    lst *l = an->d[5].p;
    size_t sb = l->l, ss = 0, se = 0;
    un sc;
    if (!sb) return chk_err(c, an, e, "chk inv cs aply");
    for (;;) {
        sc = c4_g((char*) s->d, ss, &se);
        if (sc.c.a == '\0') break;
        if (sc.c.a == '{') {
            ss = se + 1;
            sc = c4_g((char*) s->d, ss, &se);
            if (sc.c.a == '}') {
                ss = se + 1;
                sb--;
            }
        }
        ss = se + 1;
    }
    if (sb) return chk_err(c, an, e, "check inv cs fmt");
    mc *ns = mc_i(s->l + l->l * 5, s->af); // x bytes per {}
    te *h = l->h, *tn = NULL;
    ss = 0, se = 0;
    const char *rs;
    while (h) {
        for (;;) {
            sc = c4_g((char*) s->d, ss, &se);
            if (sc.c.a == '{') {
                while (sc.c.a != '}') {
                    ss = se + 1;
                    sc = c4_g((char*) s->d, ss, &se);
                    if (ast_g_t(h->d[0].p, &tn) != AST_STAT(OK) || !tn) return chk_err(c, an, e, "chk cs replace type nf");
                    if (!(rs = aply_cs_str[tn->d[1].u4])) return chk_err(c, an, e, "chk cs inv replace str");
                    mc_wcstr(&ns, rs);
                }
                ss = se + 1;
                break;
            } else mc_wc4(&ns, sc);
            ss = se + 1;
        }
        h = h->d[2].p;
    }
    for (;;) {
        sc = c4_g((char*) s->d, ss, &se);
        mc_wc4(&ns, sc); // \0 part of len
        if (sc.c.a == '\0') break;
        ss = se + 1;
    }
    mc_f(s);
    ((te*) an->d[4].p)->d[4] = P(ns);
    an->d[3] = P(type_s_i(c->a->ta, NULL, TYPE(SG)));
    return CHK_STAT(OK);
}

static chk_stat chk_aply_e_(chk *c, te *an, err **e) {
    te *tgt = ((te*) an->d[4].p)->d[3].p, *pn;
    if (!(tgt = chk_g_pn_lte(an, tgt->d[0].p))) return chk_err(c, an, e, "chk cannot get aply tgt type");
    te_f(((te*) an->d[4].p)->d[3].p);
    ((te*) an->d[4].p)->d[3] = P(te_c(tgt));
    if (ast_g_pn(AST_CLS(L), an, &pn) != AST_STAT(OK)) return chk_err(c, an, e, "chk cannot gent aply pn");
    if (tbl_s(pn->d[3].p, tgt->d[0], &tgt) != TBL_STAT(OK)) return chk_err(c, an, e, "chk inv aply pn lst update");
    te_f(tgt);
    return chk_aply_e_fn(c, an, e);
}

te *chk_rt_err_t(const alfr *ta) {
    return type_v_i(ta, NULL, TYPE(ER), type_te_i_v(ta, NULL, 3, type_s_i(ta, NULL, TYPE(SG)), type_s_i(ta, NULL, TYPE(U6)), type_s_i(ta, NULL, TYPE(SG))));
}

static chk_stat chk_aply_vr(chk *c, te *an, err **e) {
    te *tn;
    lst *l = an->d[5].p;
    if (!l || l->l != 1) return chk_err(c, an, e, "chk inv vr access");
    if (ast_g_t(l->h->d[0].p, &tn) != AST_STAT(OK)) return chk_err(c, an, e, "chk cannot get vr access type");
    if (tn->d[1].u4 != TYPE(I6) && tn->d[1].u4 != TYPE(U6)) return chk_err(c, an, e, "chk inv vr access type");
    tbl *tt = tbl_i(c->tbla, tbl_mc_sdbm, tbl_mc_eq, lst_i(c->la, c->ta, (void*) te_f), te_i(3, c->ta, NULL));
    type_tbl_a(tt, c->ta, mc_i_cstr("e", c->a->ma), 0, chk_rt_err_t(c->ta));
    if (ast_g_t(an->d[4].p, &tn) != AST_STAT(OK)) return chk_err(c, an, e, "chk cannot get type of vr");
    type_tbl_a(tt, c->ta, mc_i_cstr("v", c->a->ma), 0, te_c(tn->d[2].p));
    an->d[3] = P(type_h_i(c->ta, NULL, TYPE(UN), tt));
    return CHK_STAT(OK);
}

static chk_stat chk_aply_cj(chk *c, te *an, err **e) {
    te *tn;
    if (ast_g_t(an->d[4].p, &tn) != AST_STAT(OK)) return chk_err(c, an, e, "chk cannot get type from cj");
    tn = tn->d[2].p;
    if (type_g_c(tn->d[1].u4) != TYPE_CLS(F)) return chk_err(c, an, e, "chk inv cj inner type");
    tbl *s = tn->d[4].p;
    if (!s || s->i->l == 0) return chk_err(c, an, e, "chk inv cj no scope");
    an->d[3] = P(te_c(tn->d[2].p));
    return CHK_STAT(OK);
}

static chk_stat chk_cst_fn_lst(chk *c, te *an, err **e) {
    tbl *lt = ((te*) an->d[6].p)->d[3].p, *ut;
    te *h = lt ? lt->i->h : NULL, *r, *rt, *kv;
    uint32_t ar = 0, ax = 0;
    while (h) {
        uint32_t flgs = ast_lst_tbl_e_g_f(h->d[0].p);
        if ((flgs & LTE_FLG(O)) && (!(flgs & LTE_FLG(F)))) return chk_err(c, an, e, "chk out of scope");
        if (flgs & LTE_FLG(L)) {
            switch (((te*) ((te*) h->d[0].p)->d[2].p)->d[1].u4) {
                case TYPE(F5):
                case TYPE(F6):
                    ast_lst_tbl_e_s_i(h->d[0].p, ax++);
                    break;
                default:
                    ast_lst_tbl_e_s_i(h->d[0].p, ar++);
                    break;
            }
        }
        h = h->d[2].p;
    }
    an->d[3] = P(te_c(((te*) an->d[5].p)->d[3].p));
    r = ((lst*) ((te*) an->d[6].p)->d[4].p)->t->d[0].p;
    if (ast_g_t(r, &rt) != AST_STAT(OK)) return chk_err(c, an, e, "chk cannot get return type");
    te **fr = (te**) &((te*) an->d[3].p)->d[2].p;
    if (*fr && (*fr)->d[1].u4 == TYPE(VD)) return CHK_STAT(OK);
    else if (rt->d[1].u4 == TYPE(SL)) {
        if (!*fr) return chk_err(c, an, e, "chk init ret un nyi");
        if ((*fr)->d[1].u4 != TYPE(UN)) return chk_err(c, an, e, "chk inv ret for sl stmt");
        ut = (*fr)->d[2].p;
        if (tbl_g_i(ut, r->d[5], &kv) != TBL_STAT(OK)) {
            if (ast_g_t(r->d[4].p, &rt) != AST_STAT(OK)) return chk_err(c, an, e, "chk inv sl inner type");
            type_tbl_a(ut, c->a->ta, mc_c(r->d[5].p), 0, te_c(rt));
            lst_s(ut->i, type_h_cmp);
        } else if (!type_eq(rt, kv->d[2].p)) return chk_err(c, an, e, "chk inv un ret type");
    } else if (!*fr) *fr = te_c(rt);
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

static chk_stat chk_cst_cj(chk *c, te *an, err **e) {
    (void) c;
    (void) e;
    te *lt = ((te*) an->d[5].p)->d[3].p, *rt = ((te*) an->d[6].p)->d[3].p;
    if (!lt->d[2].p) lt->d[2] = P(te_c(rt));
    an->d[3] = P(te_c(lt));
    return CHK_STAT(OK);
}

static chk_stat chk_cst_z(chk *c, te *an, err **e) {
    te *to = ((te*) an->d[5].p)->d[3].p, *from = ((te*) an->d[6].p)->d[3].p, *rt = NULL;
    from = from->d[2].p;
    if (to->d[1].u4 == TYPE(I6) && from->d[1].u4 == TYPE(U6)) rt = te_c(to);
    if (rt) {
        an->d[3] = P(rt);
        return CHK_STAT(OK);
    }
    return chk_err(c, an, e, "chk inv z cst");
}

chk_stat chk_op_l_z_r(chk *c, te *an, err **e) {
    te *lt = ((te*) an->d[5].p)->d[3].p, *rt;
    lt = lt->d[2].p;
    if (ast_g_t(an->d[6].p, &rt) != AST_STAT(OK)) return chk_err(c, an->d[6].p, e, "chk z cannot get op r type");
    if (!type_eq(lt, rt)) return chk_err(c, an, e, "chk z op l r type ne");
    an->d[3] = P(te_c(rt));
    return CHK_STAT(OK);
}

chk_stat chk_op_l_r_z(chk *c, te *an, err **e) {
    te *lt, *rt = ((te*) an->d[6].p)->d[3].p;
    rt = rt->d[2].p;
    if (ast_g_t(an->d[5].p, &lt) != AST_STAT(OK)) return chk_err(c, an->d[6].p, e, "chk z cannot get op l type");
    if (!type_eq(lt, rt)) return chk_err(c, an, e, "chk z op l r type ne");
    an->d[3] = P(te_c(lt));
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

static chk_stat chk_op_levrroe_vr(chk *c, te *an, err **e) {
    te *lte = ((te*) an->d[5].p)->d[3].p, *rt;
    if (ast_g_t(an->d[6].p, &rt) != AST_STAT(OK)) return chk_err(c, an, e, "chk inv cncta r type");
    if (!type_eq(((te*) lte->d[2].p)->d[2].p, rt)) return chk_err(c, an, e, "chk l vr type and r type neq");
    an->d[3] = P(te_c(lte->d[2].p));
    return CHK_STAT(OK);
}

static chk_stat chk_uner_n_un(chk *c, te *an, err **e) {
    te *u;
    if (ast_g_t(an->d[6].p, &u) != AST_STAT(OK)) return chk_err(c, an, e, "chk cannot get un type");
    te *lte = un_g_lte_ch(u, 'v');
    if (!lte) return chk_err(c, an, e, "chk inv un v type");
    an->d[3] = P(te_c(lte->d[2].p));
    if ((u = chk_g_pn_fnnf_type(an))) {
        if (u->d[2].p) return chk_err(c, an, e, "chk type uner chk nyi");
        tbl *tt = tbl_i(c->tbla, tbl_mc_sdbm, tbl_mc_eq, lst_i(c->la, c->ta, (void*) te_f), te_i(3, c->ta, NULL));
        type_tbl_a(tt, c->ta, mc_i_cstr("e", c->a->ma), 0, chk_rt_err_t(c->ta));
        u->d[2] = P(type_h_i(c->ta, NULL, TYPE(UN), tt));
    }
    return CHK_STAT(OK);
}

void chk_type(chk *c);
void chk_dfnagn(chk *c);
void chk_arith(chk *c);
void chk_dump(chk *c);

chk *chk_b(chk *c) {
    // before
    CHK_AB(c, chk_cst_fn_lst_b, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(FN), AST_CLS(L), TYPE(_A));
    CHK_AB(c, chk_cst_nf_lst_b, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(NF), AST_CLS(L), TYPE(_A));
    CHK_AB(c, chk_op_lst_lr_b, AST_CLS(O), TYPE(_N), OC(LOOP), TYPE(_A), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A));
    CHK_AB(c, chk_op_lst_lr_b, AST_CLS(O), TYPE(_N), OC(IF), TYPE(_A), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A));
    CHK_AB(c, chk_op_mtch_lst_lr_b, AST_CLS(O), TYPE(_N), OC(MTCH), TYPE(_A), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A));
    // after
    CHK_AA(c, chk_nop, AST_CLS(R), TYPE(_A), AST_CLS(A), TYPE(VD));
    CHK_AA(c, chk_nop, AST_CLS(S), TYPE(U5), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(S), TYPE(I6), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(S), TYPE(F6), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(S), TYPE(CS), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(S), TYPE(ET), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(S), TYPE(FP), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(Z), TYPE(VD), AST_CLS(O), TYPE(VD));
    CHK_AA(c, chk_z_vd, AST_CLS(Z), TYPE(_N), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_z_vd, AST_CLS(Z), TYPE(_N), AST_CLS(S), TYPE(F6));
    CHK_AA(c, chk_z_vd, AST_CLS(Z), TYPE(_N), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_z_vd, AST_CLS(Z), TYPE(_N), AST_CLS(E), TYPE(F6));
    CHK_AA(c, chk_z_vd, AST_CLS(Z), TYPE(_N), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_z_vd, AST_CLS(Z), TYPE(_N), AST_CLS(O), TYPE(U6));
    CHK_AA(c, chk_z_vd, AST_CLS(Z), TYPE(_N), AST_CLS(O), TYPE(SG));
    CHK_AA(c, chk_z_type_h, AST_CLS(Z), TYPE(_N), AST_CLS(E), TYPE(ST));
    CHK_AA(c, chk_z_type_h, AST_CLS(Z), TYPE(_N), AST_CLS(E), TYPE(ET));
    CHK_AA(c, chk_z_type_h, AST_CLS(Z), TYPE(_N), AST_CLS(E), TYPE(UN));
    CHK_AA(c, chk_z_type_h, AST_CLS(Z), TYPE(_N), AST_CLS(A), TYPE(UN));
    CHK_AA(c, chk_nop, AST_CLS(Z), TYPE(VD), AST_CLS(A), TYPE(SG));
    CHK_AA(c, chk_vec, AST_CLS(V), TYPE(_N), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_lst_l, AST_CLS(A), TYPE(_N), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_aply_e_fn, AST_CLS(A), TYPE(_N), AST_CLS(E), TYPE(FN));
    CHK_AA(c, chk_aply_e_fn, AST_CLS(A), TYPE(_N), AST_CLS(E), TYPE(NF));
    CHK_AA(c, chk_aply_s_fn, AST_CLS(A), TYPE(_N), AST_CLS(S), TYPE(FP));
    CHK_AA(c, chk_aply_ez_te, AST_CLS(A), TYPE(_N), AST_CLS(E), TYPE(TE));
    CHK_AA(c, chk_aply_z_fn, AST_CLS(A), TYPE(_N), AST_CLS(Z), TYPE(FN));
    CHK_AA(c, chk_aply_ez_te, AST_CLS(A), TYPE(_N), AST_CLS(Z), TYPE(TE));
    CHK_AA(c, chk_aply_cs, AST_CLS(A), TYPE(_N), AST_CLS(S), TYPE(CS));
    CHK_AA(c, chk_aply_e_, AST_CLS(A), TYPE(_N), AST_CLS(E), TYPE(_N));
    CHK_AA(c, chk_aply_vr, AST_CLS(A), TYPE(_N), AST_CLS(E), TYPE(VR));
    CHK_AA(c, chk_aply_cj, AST_CLS(A), TYPE(_N), AST_CLS(E), TYPE(CJ));
    CHK_AA(c, chk_nop, AST_CLS(A), TYPE(UN), AST_CLS(E), TYPE(VR));
    // ops
    CHK_AA(c, chk_cst_fn_lst, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(FN), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_cst_fn_lst, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(NF), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_set_ret_op_l, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(F6), AST_CLS(E), TYPE(U6));
    CHK_AA(c, chk_set_ret_op_l, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(U6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_set_ret_op_l, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(U6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_cst_vr_v, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(VR), AST_CLS(V), TYPE(TE));
    CHK_AA(c, chk_cst_cj, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(CJ), AST_CLS(O), TYPE(NF));
    CHK_AA(c, chk_cst_z, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(I6), AST_CLS(Z), TYPE(U6));
    CHK_AA(c, chk_l_lst_bl, AST_CLS(O), TYPE(_N), OC(LOOP), TYPE(_A), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_if, AST_CLS(O), TYPE(_N), OC(IF), TYPE(_A), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_mtch, AST_CLS(O), TYPE(_N), OC(MTCH), TYPE(_A), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_op_lr_teq_bl, AST_CLS(O), TYPE(_N), OC(EQ), TYPE(_A), AST_CLS(S), TYPE(I6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_lers_us_bl, AST_CLS(O), TYPE(_N), OC(EQ), TYPE(_A), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq_bl, AST_CLS(O), TYPE(_N), OC(NE), TYPE(_A), AST_CLS(S), TYPE(I6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq_bl, AST_CLS(O), TYPE(_N), OC(GT), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lers_us_bl, AST_CLS(O), TYPE(_N), OC(GT), TYPE(_A), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq_bl, AST_CLS(O), TYPE(_N), OC(LT), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq_bl, AST_CLS(O), TYPE(_N), OC(LT), TYPE(_A), AST_CLS(A), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lers_fs_bl, AST_CLS(O), TYPE(_N), OC(LT), TYPE(_A), AST_CLS(E), TYPE(F6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq_bl, AST_CLS(O), TYPE(_N), OC(LTE), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq_bl, AST_CLS(O), TYPE(_N), OC(LTE), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_and_or, AST_CLS(O), TYPE(_N), OC(AND), TYPE(_A), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_and_or, AST_CLS(O), TYPE(_N), OC(OR), TYPE(_A), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(SG), OC(CSG), TYPE(_A), AST_CLS(_), TYPE(_N), AST_CLS(S), TYPE(CS));
    CHK_AA(c, chk_op_levrroe_vr, AST_CLS(O), TYPE(_N), OC(CNCTA), TYPE(_A), AST_CLS(E), TYPE(VR), AST_CLS(O), TYPE(F6));
    CHK_AA(c, chk_op_levrroe_vr, AST_CLS(O), TYPE(_N), OC(CNCTA), TYPE(_A), AST_CLS(E), TYPE(VR), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_levrroe_vr, AST_CLS(O), TYPE(_N), OC(CNCTA), TYPE(_A), AST_CLS(E), TYPE(VR), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_uner_n_un, AST_CLS(O), TYPE(_N), OC(UNER), TYPE(_A), AST_CLS(_), TYPE(_N), AST_CLS(A), TYPE(UN));
    chk_type(c);
    chk_dfnagn(c);
    chk_arith(c);
    chk_dump(c);
    return c;
}
