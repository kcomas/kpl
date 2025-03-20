
#include "fld.h"

fld_stat fld_err(fld_stat stat, te *an, te **e) {
    *e = te_c(an);
    return stat;
}

static fld_stat idnt_lst_r(fld *f, te **restrict an, te **restrict e) {
    te *ln, *kv;
    if (ast_g_pn(AST_CLS(L), *an, &ln) != AST_STAT(OK)) return fld_err(FLD_STAT(INV), *an, e);
    tbl *lt;
    if (!ln->d[3].p) ln->d[3].p = f->fti();
    lt = ln->d[3].p;
    if (tbl_g_i(lt, P((*an)->d[3].p), &kv) == TBL_STAT(NF)) {
        kv = ast_lst_tbl_e_i(f->a, mc_c((*an)->d[3].p), U6(0), NULL);
        tbl_a(lt, kv);
    }
    te *en = ast_an_i(f->a, (*an)->d[0].p, (*an)->d[1].p, AST_CLS(E), P(te_c(kv)));
    te_f(*an);
    *an = en;
    return FLD_STAT(OK);
}

static bool idnt_lst_t(const te *an) {
    // TODO check if parent is a namespace op
    return an->d[2].u4 == AST_CLS(I);
}

static fld_stat e_lst_type_o_def_r(fld *f, te **restrict an, te **restrict e) {
    te *lte = te_c(((te*) (*an)->d[6].p)->d[3].p);
    te *lt = (*an)->d[6].p;
    if (lt->d[2].u4 == AST_CLS(L)) {
        lte->d[2] = P(type_s_i(f->a->ta, NULL, TYPE(_L)));
        lte->d[3] = P(te_c(lt));
    } else if (lt->d[2].u4 == AST_CLS(T)) {
        lte->d[2] = P(te_c(lt->d[3].p));
        ast_lst_tbl_e_s_f(lte, LTE_FLG(T));
    } else fld_err(FLD_STAT(INV), *an, e);
    te_f(*an);
    *an = NULL;
    return FLD_STAT(OK);
}

static bool e_lst_type_o_def_t(const te *an) {
    return an->d[4].u4 == OC(DFN) && ((te*) an->d[5].p)->d[3].u4 == AST_CLS(E) && (((te*) an->d[6].p)->d[2].u4 == AST_CLS(T) || ((te*) an->d[6].p)->d[2].u4 == AST_CLS(L));
}

static fld_stat aply_op_r(fld *f, te **restrict an, te **restrict e) {
    (void) f;
    // TODO ops that are not folded
    lst *l = lst_c((*an)->d[5].p);
    if (l->l < 1 || l-l > 2) return fld_err(FLD_STAT(INV), *an, e);
    te *on = te_c((*an)->d[4].p);
    on->d[0] = (*an)->d[0];
    te_f(*an);
    un a = P(NULL), b = P(NULL);
    lst_sb(l, &b);
    if (l->l) lst_sb(l, &a);
    lst_f(l);
    on->d[5] = a;
    ((te*) a.p)->d[0] = P(on);
    on->d[6] = b;
    if (b.p) ((te*) b.p)->d[0] = P(on);
    *an = on;
    return FLD_STAT(OK);
}

static bool aply_op_t(const te *an) {
    return an->d[2].u4 == AST_CLS(A) && an->d[4].p && ((te*) an->d[4].p)->d[2].u4 == AST_CLS(O);
}


static fld_stat z_type_i(fld *f, lst *l, te *p) {
    tbl *t = p->d[3].p = f->fti();
    un ln;
    size_t i = 0;
    while (l->l) {
        if (lst_sf(l, &ln) != LST_STAT(OK)) return FLD_STAT(INV);
        te *zn = ln.p;
        if (zn->d[2].u4 != AST_CLS(Z)) return FLD_STAT(INV);
        te *tn = zn->d[4].p;
        if (tn->d[2].u4 != AST_CLS(T)) return FLD_STAT(INV);
        ((te*) tn->d[3].p)->d[0] = P(p); // set parent
        type_tbl_a(t, f->a->ta, mc_c(zn->d[5].p), i++, te_c(tn->d[3].p));
        te_f(zn);
    }
    lst_f(l);
    return FLD_STAT(OK);
}

static fld_stat aply_type_r(fld *f, te **restrict an, te **restrict e) {
    fld_stat stat;
    lst *l = lst_c((*an)->d[5].p);
    if (!l->l) return fld_err(FLD_STAT(INV), *an, e);
    te *tn = te_c((*an)->d[4].p);
    te *t = tn->d[3].p;
    type_cls tc = type_g_c(t->d[1].u4);
    un ln;
    switch (tc) {
        case TYPE_CLS(F):
            if (lst_sb(l, &ln) != LST_STAT(OK)) return fld_err(FLD_STAT(INV), *an, e);
            te *rn = ln.p;
            if (rn->d[2].u4 != AST_CLS(T)) return fld_err(FLD_STAT(INV), *an, e);
            t->d[2] = P(te_c(rn->d[3].p));
            ((te*) t->d[2].p)->d[0] = P(t); // set parent
            te_f(rn);
            if ((stat = z_type_i(f, l, t)) != FLD_STAT(OK)) return fld_err(stat, *an, e);
            break;
        default:
            return fld_err(FLD_STAT(INV), *an, e);
    }
    tn->d[0] = (*an)->d[0];
    te_f(*an);
    *an = tn;
    return FLD_STAT(OK);
}

static bool aply_type_t(const te *an) {
    return an->d[2].u4 == AST_CLS(A) && an->d[4].p && ((te*) an->d[4].p)->d[2].u4 == AST_CLS(T);
}

static fld_stat cmd_r(fld *f, te **restrict an, te **restrict e) {
    te *nn;
    switch ((*an)->d[3].u4) {
        case CC(P1):
            nn = ast_an_i(f->a, (*an)->d[0].p, (*an)->d[1].p, AST_CLS(O), P(type_s_i(f->a->ta, NULL, TYPE(VD))), U4(OC(DUMP)), ast_an_i(f->a, (*an)->d[0].p, (*an)->d[1].p, AST_CLS(S), P(type_s_i(f->a->ta, NULL, TYPE(U5))), U5(1)), te_c((*an)->d[4].p));
            ((te*) (*an)->d[4].p)->d[0] = P(nn); // update tgt parent
            break;
        default:
            return fld_err(FLD_STAT(INV), *an, e);
    }
    te_f(*an);
    *an = nn;
    return FLD_STAT(OK);
}

static bool cmd_t(const te *an) {
    return an->d[2].u4 == AST_CLS(C) && an->d[4].p;
}

fld *fld_b(fld *f) {
    fld_a(f, AST_CLS(I), idnt_lst_t, idnt_lst_r);
    fld_a(f, AST_CLS(O), e_lst_type_o_def_t, e_lst_type_o_def_r);
    fld_a(f, AST_CLS(A), aply_op_t, aply_op_r);
    fld_a(f, AST_CLS(A), aply_type_t, aply_type_r);
    fld_a(f, AST_CLS(C), cmd_t, cmd_r);
    return f;
}
