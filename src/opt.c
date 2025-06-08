
#include "opt.h"

static fld_stat entry_o(fld *f, te **an, err **e) {
    (void) f;
    te *lte = (*an)->d[3].p;
    uint32_t flgs = ast_lst_tbl_e_g_f(lte);
    if (!flgs) return fld_err(f, *an, e, "opt var flgs inv");
    if (flgs & LTE_FLG(A)) return FLD_STAT(OK);
    if ((flgs & LTE_FLG(L) || flgs & LTE_FLG(T) || flgs & LTE_FLG(F)) && lte->r < 3) return fld_err(f, *an, e, "opt unused var");
    return FLD_STAT(OK);
}

static bool entry_t(const te *an) {
    return an->d[2].u4 == AST_CLS(E);
}

static fld_stat lst_inv_o(fld *f, te **an, err **e) {
    (void) f;
    return fld_err(f, *an, e, "opt inv lst");
}

static bool lst_inv_t(const te *an) {
    te *p = an->d[0].p;
    return an->d[2].u4 == AST_CLS(L) && (!p || (p->d[2].u4 != AST_CLS(A) && p->d[2].u4 != AST_CLS(O)));
}

static fld_stat lst_l_o(fld *f, te **an, err **e) {
    (void) f;
    (void) e;
    te *h = ((tbl*) (*an)->d[3].p)->i->h, *lte, *kv;
    while (h) {
        lte = h->d[0].p;
        if ((ast_lst_tbl_e_g_f(lte) & LTE_FLG(O)) && (kv = chk_g_pn_lte((*an)->d[0].p, lte->d[0].p))) {
            lte->d[1] = U6(kv->d[1].u6 | LTE_FLG(O));
        }
        h = h->d[2].p;
    }
    return FLD_STAT(OK);
}

static bool lst_l_t(const te *an) {
    return an->d[2].u4 == AST_CLS(L);
}

static fld_stat aply_lst_o(fld *f, te **an, err **e) {
    (void) f;
    (void) e;
    te *lp, *ln, *h, *lte;
    if (ast_g_pn(AST_CLS(L), *an, &lp) == AST_STAT(OK)) {
        HERE("TODO add to parent lst");
    } else {
        ln = te_c((*an)->d[4].p);
        ln->d[0] = (*an)->d[0];
        te_f(*an);
        *an = ln;
        uint32_t lc = 0; // update idxs of locals
        h = ((tbl*) ln->d[3].p)->i->h;
        while (h) {
            lte = h->d[0].p;
            if (ast_lst_tbl_e_g_f(lte) & LTE_FLG(L)) ast_lst_tbl_e_s_i(lte, lc++);
            h = h->d[2].p;
        }
    }
    return FLD_STAT(OK);
}

static bool aply_lst_t(const te *an) {
    return an->d[2].u4 == AST_CLS(A) && an->d[4].p && ((te*) an->d[4].p)->d[2].u4 == AST_CLS(L);
}

static fld_stat cst_s_o(fld *f, te **an, err **e) {
    te *lt = ((te*) (*an)->d[5].p)->d[3].p, *r = (*an)->d[6].p, *rt = r->d[3].p;
    if (lt->d[1].u4 == rt->d[1].u4) return fld_err(f, *an, e, "opt unnecessary cst");
    switch (lt->d[1].u4) {
        case TYPE(U6):
            switch (rt->d[1].u4) {
                case TYPE(I6):
                    rt->d[1].u4 = TYPE(U6);
                    break;
                default:
                    return fld_err(f, *an, e, "opt inv cst U5");
            }
            break;
        default:
            return fld_err(f, *an, e, "opt inv cst s");
    }
    r->d[0] = (*an)->d[0];
    te_c(r);
    te_f(*an);
    *an = r;
    return FLD_STAT(OK);
}

static bool cst_s_t(const te *an) {
    return an->d[2].u4 == AST_CLS(O) && an->d[4].u4 == OC(CST) && ((te*) an->d[5].p)->d[2].u4 == AST_CLS(T) && ((te*) an->d[6].p)->d[2].u4 == AST_CLS(S);
}

fld *opt_b(fld *f) {
    fld_a(f, AST_CLS(E), entry_t, entry_o);
    fld_a(f, AST_CLS(L), lst_inv_t, lst_inv_o);
    fld_a(f, AST_CLS(L), lst_l_t, lst_l_o);
    fld_a(f, AST_CLS(A), aply_lst_t, aply_lst_o);
    fld_a(f, AST_CLS(O), cst_s_t, cst_s_o);
    return f;
}
