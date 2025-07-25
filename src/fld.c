
#include "fld.h"

fld_stat fld_err(const fld *f, te *an, err **e, const char *m) {
    te_c(ast_g_root(an));
    *e = err_i(f->ea, ast_err_p, ast_err_f, an, m);
    return FLD_STAT(INV);
}

static fld_stat lst_r(fld *f, te **an, err **e) {
    return fld_err(f, *an, e, "fld bare lst");
}

static bool lst_t(const te *an) {
    te *pn = an->d[0].p;
    return pn->d[2].u4 == AST_CLS(L);
}

static fld_stat s_cs_sg_r(fld *f, te **an, err **e) {
    (void) e;
    te *nn = ast_an_i(f->a, (*an)->d[0].p, (*an)->d[1].p, AST_CLS(O), P(type_s_i(f->a->ta, NULL, TYPE(SG))), U4(OC(CSG)), NULL, *an);
    (*an)->d[0] = P(nn); // update tgt parent
    *an = nn;
    return FLD_STAT(OK);
}

static bool s_cs_sg_t(const te *an) {
    te *pn;
    if (!an->d[3].p || ((te*) an->d[3].p)->d[1].u4 != TYPE(CS)) return false;
    pn = an->d[0].p;
    if (!pn || (pn->d[2].u4 == AST_CLS(A) && pn->d[4].p == an)) return false;
    return pn->d[2].u4 != AST_CLS(C) || pn->d[3].u4 != CC(L);
}

static fld_stat idnt_lst_r(fld *f, te **an, err **e) {
    te *ln, *kv;
    if (ast_g_pn(AST_CLS(L), *an, &ln) != AST_STAT(OK)) return fld_err(f, *an, e, "fld ast_g_pn");
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
    te *pn = an->d[0].p;
    if (pn->d[2].u4 == AST_CLS(A)) {
        if (an != pn->d[4].p) return ((te*) pn->d[4].p)->d[2].u4 != AST_CLS(O) || ((te*) pn->d[4].p)->d[4].u4 != OC(NS);
        pn = pn->d[0].p;
    }
    return pn->d[2].u4 != AST_CLS(O) || pn->d[4].u4 != OC(NS);
}

static fld_stat enlst(fld *f, te **an, err **e) {
    (void) e;
    if (*an && (*an)->d[2].u4 == AST_CLS(L)) return FLD_STAT(OK);
    if (!*an) {
        *an = ast_an_i(f->a, NULL, NULL, AST_CLS(L), P(NULL), NULL);
        return FLD_STAT(OK);
    }
    lst *ll = f->a->ali();
    te *ln = ast_an_i(f->a, (*an)->d[0].p, (*an)->d[1].p, AST_CLS(L), P(NULL), ll);
    lst_ab(ll, P(*an));
    (*an)->d[0] = P(ln);
    *an = ln;
    return FLD_STAT(OK);
}

static fld_stat op_lr_lst_r(fld *f, te **an, err **e) {
    fld_stat fstat;
    if ((fstat = enlst(f, (te**) &(*an)->d[5].p, e)) != FLD_STAT(OK)) return fstat;
    return enlst(f, (te**) &(*an)->d[6].p, e);
}

static bool lr_lst_t(const te *an) {
    switch (an->d[4].u4) {
        case OC(LOOP):
        case OC(IF):
        case OC(MTCH):
        case OC(OR):
        case OC(AND):
            break;
        default:
            return false;
    }
    return true;
}

static bool op_lr_lst_t(const te *an) {
    te *l = an->d[5].p, *r = an->d[6].p;
    if ((!l && !r) || !lr_lst_t(an)) return false;
    return (!l || l->d[2].u4 != AST_CLS(L)) || (!r || r->d[2].u4 != AST_CLS(L));
}

static fld_stat scope(fld *f, te *an, err **e) {
    te *pl, *h, *kv;
    if (!an->d[4].p) return FLD_STAT(OK);
    if (ast_g_pn(AST_CLS(L), an->d[0].p, &pl) != AST_STAT(OK)) return fld_err(f, an, e, "fld ast_g_pn");
    if (!pl->d[3].p) {
        pl->d[3] = an->d[3];
        an->d[3] = P(NULL);
    } else if (an->d[3].p) {
        h = ((tbl*) an->d[3].p)->i->h;
        while (h) {
            if (tbl_g_i(pl->d[3].p, ((te*) h->d[0].p)->d[0], &kv) == TBL_STAT(NF)) tbl_a(pl->d[3].p, te_c(h->d[0].p));
            h = h->d[2].p;
        }
    }
    return FLD_STAT(OK);
}

static fld_stat op_lr_lst_scope_r(fld *f, te **an, err **e) {
    fld_stat fstat;
    if ((fstat = scope(f, (*an)->d[5].p, e)) != FLD_STAT(OK)) return fstat;
    return scope(f, (*an)->d[6].p, e);
}

static bool op_lr_lst_scope_t(const te *an) {
    te *l = an->d[5].p, *r = an->d[6].p;
    if ((!l && !r) || !lr_lst_t(an)) return false;
    if (an->d[2].u4 == AST_CLS(O) && an->d[4].u4 == OC(MTCH)) return !l->d[4].p;
    return l->d[2].u4 == AST_CLS(L) && r->d[2].u4 == AST_CLS(L);
}

static size_t fld_ns_add(te *an, size_t n, mc *m[NS_MAX_L]) {
    if (!an || an->d[2].u4 != AST_CLS(I)) return n;
    m[n++] = mc_c(an->d[3].p);
    return n;
}

static fld_stat op_ns_r(fld *f, te **an, err **e) {
    mc *m[NS_MAX_L];
    size_t n = 0;
    te *cn = *an, *nn;
    while (cn && cn->d[2].u4 == AST_CLS(O) && cn->d[4].u4 == OC(NS)) {
        if (n >= NS_MAX_L) return fld_err(f, *an, e, "fld ns name too long");
        n = fld_ns_add(cn->d[5].p, n, m);
        n = fld_ns_add(cn->d[6].p, n, m);
        cn = cn->d[6].p;
    }
    if (cn && cn->d[2].u4 == AST_CLS(A)) {
        nn = cn->d[4].p;
        if (nn->d[2].u4 != AST_CLS(I)) return fld_err(f, *an, e, "fld ns inv tgt");
        m[n++] = mc_c(nn->d[3].p);
        if (n >= NS_MAX_L) return fld_err(f, *an, e, "fld ns name too long");
        if (!(nn = ns_n(n, m))) return fld_err(f, *an, e, "fld inv ns");
        nn = ast_an_i(f->a, cn, (*an)->d[1].p, AST_CLS(S), P(te_c(nn->d[1].p)), nn->d[2].p);
        te_f(cn->d[4].p);
        cn->d[4] = P(nn);
        cn->d[0] = (*an)->d[0];
    } else if (!cn) {
        if (!(nn = ns_n(n, m))) return fld_err(f, *an, e, "fld inv ns");
        cn = ast_an_i(f->a, (*an)->d[0].p, (*an)->d[1].p, AST_CLS(S), P(te_c(nn->d[1].p)), nn->d[2].p);
    } else return fld_err(f, *an, e, "fld ns inv");
    te_c(cn);
    te_f(*an);
    *an = cn;
    for (size_t i = 0; i < n; i++) mc_f(m[i]);
    return FLD_STAT(OK);
}

static bool op_ns_t(const te *an) {
    if (an->d[4].u4 != OC(NS)) return false;
    te *pn = an->d[0].p;
    if (pn->d[2].u4 == AST_CLS(A)) pn = pn->d[4].p;
    if (pn->d[2].u4 == AST_CLS(O) && pn->d[4].u4 == OC(NS)) return false;
    return true;
}

static fld_stat cst_cj_lst_r(fld *f, te **an, err **e) {
    (void) e;
    te *r = (*an)->d[6].p, *nn;
    nn = ast_an_i(f->a, (*an)->d[0].p, (*an)->d[1].p, AST_CLS(O), P(NULL), OC(CST), ast_an_i(f->a, (*an)->d[0].p, (*an)->d[1].p, AST_CLS(T), P(type_f_i(f->a->ta, NULL, TYPE(NF), NULL, NULL, NULL))), P(r));
    r->d[0] = P(nn);
    if (fld_tmp_var_a(f, &nn, e, --f->tvc, LTE_FLG(F)) != FLD_STAT(OK)) return fld_err(f, *an, e, "fld inv cj cst nf fn def");
    (*an)->d[6] = P(nn);
    return FLD_STAT(OK);
}

static bool cst_cj_lst_t(const te *an) {
    if (an->d[4].u4 != OC(CST)) return false;
    te *tn = an->d[5].p;
    if (!tn || tn->d[2].u4 != AST_CLS(T)) return false;
    tn = tn->d[3].p;
    if (!tn || tn->d[1].u4 != TYPE(CJ)) return false;
    tn = an->d[6].p;
    return tn && tn->d[2].u4 == AST_CLS(L);
}

fld_stat fld_tmp_var_a(fld *f, te **an, err **e, int32_t vi, lte_flg vf) {
    te *pn = *an, *ln = NULL, *kv, *en;
    while (ast_g_pn(AST_CLS(O), pn->d[0].p, &pn) == AST_STAT(OK)) {
        if (pn->d[4].u4 != OC(CST)) continue;
        kv = pn->d[5].p;
        if (kv->d[2].u4 != AST_CLS(T)) continue;
        kv = kv->d[3].p;
        if (type_g_c(kv->d[1].u4) != TYPE_CLS(F)) continue;
        ln = pn->d[6].p;
        break;
    }
    if (!ln) {
        if (ast_g_pn(AST_CLS(R), *an, &ln) != AST_STAT(OK)) return fld_err(f, *an, e, "opt tmp var inv root");
        ln = ((te*) ln->d[4].p)->d[4].p;
    }
    if (!ln->d[3].p) ln->d[3] = P(f->fti());
    mc *tv = mc_i(f->tvc % 10 + 5, &al_mc);
    ssize_t r;
    if ((r = snprintf((char*) tv->d, tv->s, "%d", vi)) < 1) return fld_err(f, *an, e, "opt inv tmp var str");
    tv->l = (size_t) r + 1;
    if ((*an)->d[3].p) kv = ast_lst_tbl_e_i(f->a, tv, U6(vf), te_c((*an)->d[3].p));
    else kv = ast_lst_tbl_e_i(f->a, tv, U6(0), NULL);
    tbl_a(ln->d[3].p, kv);
    en = ast_an_i(f->a, (*an)->d[0].p, (*an)->d[1].p, AST_CLS(E), P(te_c(kv)));
    *an = ast_an_i(f->a, (*an)->d[0].p, (*an)->d[1].p, AST_CLS(O), P((*an)->d[3].p ? te_c((*an)->d[3].p) : NULL), U4(OC(DFN)), en, *an);
    ((te*) (*an)->d[6].p)->d[0] = P(*an);
    return FLD_STAT(OK);
}

static fld_stat aply_op_r(fld *f, te **an, err **e) {
    (void) f;
    // TODO ops that are not folded
    lst *l = lst_c((*an)->d[5].p);
    if (l->l < 1 || l-l > 2) return fld_err(f, *an, e, "fld aply op inv num args");
    te *on = te_c((*an)->d[4].p);
    on->d[0] = (*an)->d[0];
    on->d[1] = (*an)->d[1]; // for psr errors
    te_f(*an);
    un a = P(NULL), b = P(NULL);
    lst_sb(l, &b);
    if (l->l) lst_sb(l, &a);
    lst_f(l);
    on->d[5] = a;
    if (a.p) ((te*) a.p)->d[0] = P(on);
    on->d[6] = b;
    if (b.p) ((te*) b.p)->d[0] = P(on);
    *an = on;
    return FLD_STAT(OK);
}

static bool aply_op_t(const te *an) {
    return an->d[2].u4 == AST_CLS(A) && an->d[4].p && ((te*) an->d[4].p)->d[2].u4 == AST_CLS(O);
}

static fld_stat aply_type_e_r(fld *f, te **an, err **e) {
    te *t = ((te*) (*an)->d[4].p)->d[3].p;
    te *en = ((lst*) (*an)->d[5].p)->h->d[0].p;
    te *lte = en->d[3].p;
    if (lte->d[2].p) return fld_err(f, *an, e, "fld lte type set");
    lte->d[2] = P(te_c(t));
    te_c(en);
    en->d[0] = (*an)->d[0];
    te_f(*an);
    *an = en;
    return FLD_STAT(OK);
}

static bool aply_type_e_t(const te *an) {
    if (an->d[2].u4 != AST_CLS(A)) return false;
    te *a4 = an->d[4].p;
    if (!a4 || a4->d[2].u4 != AST_CLS(T)) return false;
    lst *l = an->d[5].p;
    if (!l || l->l != 1) return false;
    te *e = l->h->d[0].p;
    if (!e || e->d[2].u4 != AST_CLS(E)) return false;
    return true;
}

static fld_stat z_type_i_fn(fld *f, lst *l, te *p) {
    if (!l->l) {
        lst_f(l);
        return FLD_STAT(OK);
    }
    tbl *t = p->d[3].p = f->fti();
    un ln;
    size_t ra = 0, xa = 0;
    while (l->l) {
        if (lst_sf(l, &ln) != LST_STAT(OK)) return FLD_STAT(INV);
        te *zn = ln.p;
        if (zn->d[2].u4 != AST_CLS(Z)) return FLD_STAT(INV);
        te *tn = zn->d[4].p;
        if (tn->d[2].u4 != AST_CLS(T)) return FLD_STAT(INV);
        ((te*) tn->d[3].p)->d[0] = P(p); // set parent
        switch (((te*) tn->d[3].p)->d[1].u4) {
            case TYPE(F5):
            case TYPE(F6):
                type_tbl_a(t, f->a->ta, mc_c(zn->d[5].p), xa++, te_c(tn->d[3].p));
                break;
            default:
                type_tbl_a(t, f->a->ta, mc_c(zn->d[5].p), ra++, te_c(tn->d[3].p));
                break;
        }
        te_f(zn);
    }
    lst_f(l);
    return FLD_STAT(OK);
}

static fld_stat z_type_i_h(fld *f, lst *l, te *p) {
    if (!l->l) {
        lst_f(l);
        return FLD_STAT(OK);
    }
    tbl *t = p->d[2].p = f->fti();
    un ln;
    while (l->l) {
        if (lst_sf(l, &ln) != LST_STAT(OK)) return FLD_STAT(INV);
        te *zn = ln.p;
        if (zn->d[2].u4 != AST_CLS(Z)) return FLD_STAT(INV);
        te *tn = zn->d[4].p;
        if (tn->d[2].u4 != AST_CLS(T)) return FLD_STAT(INV);
        ((te*) tn->d[3].p)->d[0] = P(p); // set parent
        type_tbl_a(t, f->a->ta, mc_c(zn->d[5].p), 0, te_c(tn->d[3].p));
        te_f(zn);
    }
    lst_f(l);
    lst_s(t->i, type_h_cmp);
    return FLD_STAT(OK);
}

static fld_stat aply_type_b_r(fld *f, te **an, err **e) {
    fld_stat stat;
    lst *l = lst_c((*an)->d[5].p);
    if (!l->l) return fld_err(f, *an, e, "fld aply lst len inv");
    te *tn = te_c((*an)->d[4].p), *t = tn->d[3].p, *n;
    un ln;
    switch (type_g_c(t->d[1].u4)) {
        case TYPE_CLS(V):
            if (l->l != 1) return fld_err(f, *an, e, "fld inv vr type");
            n = l->h->d[0].p;
            if (n->d[2].u4 != AST_CLS(T)) return fld_err(f, *an, e, "fld inv vr aply type");
            t->d[2] = P(te_c(n->d[3].p));
            lst_f(l);
            break;
        case TYPE_CLS(H):
            if ((stat = z_type_i_h(f, l, t)) != FLD_STAT(OK)) return fld_err(f, *an, e, "fld inv h type aply");
            break;
        case TYPE_CLS(F):
            if (lst_sb(l, &ln) != LST_STAT(OK)) return fld_err(f, *an, e, "fld inv fn type len");
            te *rn = ln.p;
            if (rn->d[2].u4 != AST_CLS(T)) return fld_err(f, *an, e, "fld inv arg type");
            if (rn->d[3].p) {
                t->d[2] = P(te_c(rn->d[3].p));
                ((te*) t->d[2].p)->d[0] = P(t); // set parent
            }
            te_f(rn);
            if ((stat = z_type_i_fn(f, l, t)) != FLD_STAT(OK)) return fld_err(f, *an, e, "fld inv fn type args");
            break;
        default:
            return fld_err(f, *an, e, "fld inv aply type");
    }
    tn->d[0] = (*an)->d[0];
    te_f(*an);
    *an = tn;
    return FLD_STAT(OK);
}

static bool aply_type_b_t(const te *an) {
    return an->d[2].u4 == AST_CLS(A) && an->d[4].p && ((te*) an->d[4].p)->d[2].u4 == AST_CLS(T);
}

void fld_s_st_et_f(void *p) {
    te *n = p;
    tbl *et = ((te*) n->d[3].p)->d[2].p;
    opt_exp_tbl_f(et);
    ((te*) n->d[3].p)->d[2].p = NULL;
    te_f(n->d[3].p);
    n->af->f(n);
}

static fld_stat cmd_r(fld *f, te **an, err **e) {
    te *nn, *en;
    tbl *et = NULL;
    switch ((*an)->d[3].u4) {
        case CC(E):
            if (((te*) (*an)->d[4].p)->d[2].u4 != AST_CLS(E)) return fld_err(f, *an, e, "fld not exportable entry");
            nn = te_c((*an)->d[4].p);
            ast_lst_tbl_e_s_f(nn->d[3].p, LTE_FLG(E));
            break;
        case CC(L):
            nn = (*an)->d[4].p;
            if (nn->d[2].u4 != AST_CLS(S) || ((te*) nn->d[3].p)->d[1].u4 != TYPE(CS)) return fld_err(f, *an, e, "fld CMD L inv import");
            if ((*e = z(nn->d[4].p, &et, 0))) return FLD_STAT(INV);
            if (!et) return fld_err(f, *an, e, "fld file has no exports");
            nn = ast_s_i(f->a, (*an)->d[0].p, (*an)->d[1].p, fld_s_st_et_f, P(type_h_i(f->a->ta, NULL, TYPE(ET), et)), P(NULL));
            break;
        case CC(T):
            en = (*an)->d[4].p;
            if (!en || en->d[2].u4 != AST_CLS(E)) return fld_err(f, *an, e, "fld inv type cmd");
            nn = ast_an_i(f->a, (*an)->d[0].p, (*an)->d[1].p, AST_CLS(T), P(type_rf_i(f->a->ta, (te**) &((te*) en->d[3].p)->d[2].p, 0)));
            break;
        case CC(P1):
            nn = ast_an_i(f->a, (*an)->d[0].p, (*an)->d[1].p, AST_CLS(O), P(type_s_i(f->a->ta, NULL, TYPE(VD))), U4(OC(DUMP)), ast_an_i(f->a, (*an)->d[0].p, (*an)->d[1].p, AST_CLS(S), P(type_s_i(f->a->ta, NULL, TYPE(U5))), U5(1)), te_c((*an)->d[4].p));
            ((te*) (*an)->d[4].p)->d[0] = P(nn); // update tgt parent
            break;
        default:
            return fld_err(f, *an, e, "fld inv cmd");
    }
    te_f(*an);
    *an = nn;
    return FLD_STAT(OK);
}

static bool cmd_t(const te *an) {
    return an->d[2].u4 == AST_CLS(C) && an->d[4].p;
}

fld *fld_b(fld *f) {
    fld_a(f, AST_CLS(L), lst_t, lst_r);
    fld_a(f, AST_CLS(S), s_cs_sg_t, s_cs_sg_r);
    fld_a(f, AST_CLS(I), idnt_lst_t, idnt_lst_r);
    fld_a(f, AST_CLS(O), op_lr_lst_t, op_lr_lst_r);
    fld_a(f, AST_CLS(O), op_lr_lst_scope_t, op_lr_lst_scope_r);
    fld_a(f, AST_CLS(O), op_ns_t, op_ns_r);
    fld_a(f, AST_CLS(O), cst_cj_lst_t, cst_cj_lst_r);
    fld_a(f, AST_CLS(A), aply_op_t, aply_op_r);
    fld_a(f, AST_CLS(A), aply_type_e_t, aply_type_e_r);
    fld_a(f, AST_CLS(A), aply_type_b_t, aply_type_b_r);
    fld_a(f, AST_CLS(C), cmd_t, cmd_r);
    return f;
}
