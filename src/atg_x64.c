
#include "atg_x64.h"

static void atg_add_e_p(const tbl *t, size_t n, size_t idnt) {
    n--;
    te *h = t->i->h;
    while (h) {
        for (size_t i = 0; i < idnt; i++) putchar(' ');
        te *kv = h->d[0].p;
        uint16_t hc = u4_g_o(kv->d[0], AST_HSH_C);
        uint16_t ht = u4_g_o(kv->d[0], AST_HSH_T);
        printf("[%s %s]", ast_cls_str(hc), type_str(ht));
        if (n > 0) {
            putchar('\n');
            atg_add_e_p(kv->d[1].p, n, idnt + 1);
        }
        h = h->d[2].p;
        if (h) putchar('\n');
    }
}

void atg_tbl_p(const tbl *t, ast_cls cls, size_t idnt) {
    te *h = t->i->h;
    while (h) {
        te *kv = h->d[0].p;
        uint16_t hc = u4_g_o(kv->d[0], 1);
        uint16_t ht = u4_g_o(kv->d[0], 0);
        putchar('(');
        switch (cls) {
            case AST_CLS(O):
                printf("[%s %s]\n", ast_oc_str(hc), type_str(ht));
                atg_add_e_p(kv->d[1].p, 2, idnt + 1);
                break;
            default:
                printf("INV ATG P)\n");
                return;
        }
        printf(")\n");
        h = h->d[2].p;
    }
}

static bool dfn_cst_type_lst_t(const te *an) {
    te *pn = an->d[0].p;
    return an->d[2].u4 == AST_CLS(O) && an->d[4].u4 == OC(CST) && an->d[6].p && ((te*) an->d[6].p)->d[2].u4 == AST_CLS(L) && pn->d[2].u4 == AST_CLS(O) && pn->d[4].u4 == OC(DFN) && ((te*) pn->d[5].p)->d[2].u4 == AST_CLS(E);
}

static bool root_lst_t(const te *an) {
    return an->d[2].u4 == AST_CLS(L) && an->d[0].p && ((te*) an->d[0].p)->d[2].u4 == AST_CLS(R);
}

bool atg_x64_enq(const te *an) {
    switch (an->d[2].u4) {
        case AST_CLS(O):
            return dfn_cst_type_lst_t(an);
        case AST_CLS(L):
            return root_lst_t(an);
        default:
            break;
    }
    return false;
}

x64_type atg_x64_g_t(const te *type) {
    if (!type) return X64_TYPE(N);
    switch (type->d[1].u4) {
        case TYPE(U3): return X64_TYPE(U3);
        case TYPE(U4): return X64_TYPE(U4);
        case TYPE(U5): return X64_TYPE(U5);
        case TYPE(U6): return X64_TYPE(U6);
        case TYPE(I3): return X64_TYPE(I3);
        case TYPE(I4): return X64_TYPE(I4);
        case TYPE(I5): return X64_TYPE(I5);
        case TYPE(I6): return X64_TYPE(I6);
        case TYPE(F5): return X64_TYPE(F5);
        case TYPE(F6): return X64_TYPE(F6);
        // TODO
        default:
            break;
    }
    return X64_TYPE(N);
}

atg_stat atg_err(const atg *t, te *an, err **e, const char *m) {
    te_c(ast_g_root(an));
    *e = err_i(t->ea, ast_err_p, ast_err_f, an, m);
    return ATG_STAT(INV);
}

static atg_stat cst_type_lst_s(atg *t, gen *g, te *an, err **e) {
    te *lte = ((te*) ((te*) an->d[0].p)->d[5].p)->d[3].p;
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, ast_lst_tbl_e_g_i(lte)), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    // TODO zero ref counted vars
    return ATG_STAT(OK);
}

static atg_stat cst_type_lst_e(atg *t, gen *g, te *an, err **e) {
    // TODO gc ref counted vars
    te *rt = ((te*) ((te*) an->d[5].p)->d[3].p)->d[2].p, *ct = g->code->t, *ret;
    if (rt->d[1].u4 != TYPE(VD)) {
        while (((te*) ct->d[0].p)->d[0].u4 == GEN_OP(LBL)) ct = ct->d[1].p;
        ret = ((te*) ct->d[0].p)->d[1].p;
        if (!ret) return atg_err(t, an, e, "atg inv ret for LEAVE");
        if (gen_a(g, GEN_OP(LEAVE), te_c(ret), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    } else if (gen_a(g, GEN_OP(LEAVE), NULL, NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat root_lst_s(atg *t, gen *g, te *an, err **e) {
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, g->lbl = t->lc++), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    // TODO zero ref counted vars
    return ATG_STAT(OK);
}

static x64_type type_g_x64_type(te *tn) {
    switch (tn->d[1].u4) {
        case TYPE(I3): return X64_TYPE(I3);
        case TYPE(I4): return X64_TYPE(I4);
        case TYPE(I5): return X64_TYPE(I5);
        case TYPE(I6): return X64_TYPE(I6);
        case TYPE(U3): return X64_TYPE(U3);
        case TYPE(U4): return X64_TYPE(U4);
        case TYPE(U5): return X64_TYPE(U5);
        case TYPE(U6): return X64_TYPE(U6);
        default:
            break;
    }
    return X64_TYPE(N);
}

#define ATG_EXP_ARG_ID 0

static atg_stat root_lst_e(atg *t, gen *g, te *an, err **e) {
    te *rn = an->d[0].p;
    if (rn->d[3].p) {
        te *h = ((tbl*) rn->d[3].p)->i->h, *d, *es;
        while (h) {
            d = h->d[0].p;
            uint32_t id = opt_exp_id(d);
            uint16_t eid = opt_exp_eid(d), flgs = opt_exp_flgs(d);
            x64_type xt = type_g_x64_type(d->d[2].p);
            if (flgs & LTE_FLG(A)) es = gen_arg(g, xt, id);
            else if (flgs & LTE_FLG(L)) es = gen_stkv(g, xt, id);
            else return atg_err(t, an, e, "atg inv exp type");
            uint32_t idx = sizeof(void*) * 4 + sizeof(void*) * eid;
            // TODO build more complex types
            if (gen_a(g, GEN_OP(SET), gen_idx_m(g, xt, 2, gen_arg(g, X64_TYPE(M), ATG_EXP_ARG_ID), gen_data(g, idx <= INT8_MAX ? X64_TYPE(U3) : X64_TYPE(U5), U5(idx))), es, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
            h = h->d[2].p;
        }
    }
    if (gen_a(g, GEN_OP(LEAVE), gen_data(g, X64_TYPE(M), P(NULL)), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

te *var_arg(gen *g, te *lte, x64_type xt) {
    uint32_t id = ast_lst_tbl_e_g_i(lte);
    uint32_t flgs = ast_lst_tbl_e_g_f(lte);
    if (flgs & LTE_FLG(A)) return gen_arg(g, xt, id);
    if (flgs & LTE_FLG(L)) return gen_stkv(g, xt, id);
    if ((xt = x64_type_to_ref(xt)) == X64_TYPE(N)) return NULL;
    if (flgs & LTE_FLG(S)) return gen_stka(g, xt, id);
    return NULL;
}

static atg_stat atg_ok(atg *t, gen *g, te *an, err **e) {
    (void) t;
    (void) g;
    (void) an;
    (void) e;
    return ATG_STAT(OK);
}

static atg_stat cst_f6_e_u6(atg *t, gen *g, te *an, err **e) {
    te *r = ((te*) an->d[6].p)->d[3].p;
    if (!(r = var_arg(g, r, X64_TYPE(U6)))) return atg_err(t, an->d[6].p, e, "atg cst r inv");
    if (gen_a(g, GEN_OP(CST), gen_tmp(g, X64_TYPE(F6), t->tc++), r, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static gen_op atg_gen_g_cond_rv(const te *ci) {
    switch (ci->d[0].u6) {
        case GEN_OP(LT): return GEN_OP(GTE);
        case GEN_OP(LTE): return GEN_OP(GT);
        case GEN_OP(GT): return GEN_OP(LTE);
        case GEN_OP(GTE): return GEN_OP(LT);
        default:
            break;
    }
    return GEN_OP(_END);
}

static atg_stat cond_s_lbl(te *cnds, uint32_t l) {
    if (!cnds) return ATG_STAT(INV);
    while (cnds) {
        te *lbl = ((te*) cnds->d[0].p)->d[3].p;
        if (gen_var_g_c(lbl) != GEN_CLS(L)) return ATG_STAT(INV);
        lbl->d[1] = U5(l);
        cnds = cnds->d[2].p;
    }
    return ATG_STAT(OK);
}

static atg_stat loop_l_l(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    uint32_t sl = t->lc++, el = t->lc++;
    te *cnds = g->code->t, *ende, *ci;
    if ((stat = atg_r(t, g, an->d[5].p, e)) != ATG_STAT(OK)) return stat;
    cnds = cnds->d[2].p; // gen code gen by cond
    if ((stat = cond_s_lbl(cnds, el)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg loop cond not lbl");
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, sl), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    ende = g->code->t;
    if ((stat = atg_r(t, g, an->d[6].p, e)) != ATG_STAT(OK)) return stat;
    while (cnds != ende) {
        ci = cnds->d[0].p;
        if (gen_a(g, atg_gen_g_cond_rv(ci), te_c(ci->d[1].p), te_c(ci->d[2].p), gen_lbl(g, sl)) != GEN_STAT(OK)) return atg_err(t, an, e, "atg cannot invert loop logic");
        cnds = cnds->d[2].p;
    }
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, el), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat if_cond(atg *t, gen *g, te *an, err **e, uint32_t tl) {
    atg_stat stat;
    te *h;
    if (an->d[2].u4 == AST_CLS(S)) {
        x64_type xt = type_g_x64_type(an->d[3].p);
        if (gen_a(g, GEN_OP(SET), gen_tmp(g, xt, tl), gen_data(g, xt, an->d[4]), NULL)) return atg_err(t, an, e, __FUNCTION__);
    } else {
        if (an->d[2].u4 == AST_CLS(L)) {
            h = ((lst*) an->d[4].p)->h;
            while (h) {
                if ((stat = if_cond(t, g, h->d[0].p, e, tl)) != ATG_STAT(OK)) return stat;
                h= h->d[2].p;
            }
        } else if ((stat = atg_r(t, g, an, e)) != ATG_STAT(OK)) return stat;
        te *c = g->code->t->d[0].p;
        if (gen_var_g_c(c->d[1].p) != GEN_CLS(T)) return atg_err(t, an, e, "atg inv if case");
        ((te*) c->d[1].p)->d[1] = U5(tl);
    }
    return ATG_STAT(OK);
}

static atg_stat if_l_l_2(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    uint32_t sl = t->lc++, ml = t->lc++, el = t->lc++, tl = t->tc++;
    te *c = g->code->t;
    if ((stat = atg_r(t, g, an->d[5].p, e)) != ATG_STAT(OK)) return stat;
    c = c->d[2].p; // gen code gen by cond
    if ((stat = cond_s_lbl(c, ml)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg if cond not lbl");
    lst *bl = ((te*) an->d[6].p)->d[4].p;
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, sl), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if ((stat = if_cond(t, g, bl->h->d[0].p, e, tl)) != ATG_STAT(OK)) return stat;
    if (gen_a(g, GEN_OP(JMP), gen_lbl(g, el), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, ml), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if ((stat = if_cond(t, g, bl->t->d[0].p, e, tl)) != ATG_STAT(OK)) return stat;
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, el), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat if_l_l_n(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    uint32_t sl, ml, el = t->lc++, tl = t->tc++;
    lst *bl = ((te*) an->d[6].p)->d[4].p;
    te *h = bl->h, *c, *l, *r;
    while (h != bl->t) {
        l = ((te*) h->d[0].p)->d[5].p;
        r = ((te*) h->d[0].p)->d[6].p;
        sl = t->lc++;
        ml = t->lc++;
        c = g->code->t;
        if ((stat = atg_lst_r(t, g, l->d[4].p, e)) != ATG_STAT(OK)) return stat;
        c = c->d[2].p; // gen code gen by cond
        if ((stat = cond_s_lbl(c, ml)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg if cond not lbl");
        if (gen_a(g, GEN_OP(LBL), gen_lbl(g, sl), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        if ((stat = if_cond(t, g, r, e, tl)) != ATG_STAT(OK)) return stat;
        if (gen_a(g, GEN_OP(JMP), gen_lbl(g, el), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        if (gen_a(g, GEN_OP(LBL), gen_lbl(g, ml), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        h = h->d[2].p;
    }
    if ((stat = if_cond(t, g, bl->t->d[0].p, e, tl)) != ATG_STAT(OK)) return stat;
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, el), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat if_l_l(atg *t, gen *g, te *an, err **e) {
    lst *ic = ((te*) an->d[5].p)->d[4].p;
    lst *ib = ((te*) an->d[6].p)->d[4].p;
    if (ic && ib->l == 1) return atg_err(t, an, e, "nyi");
    else if (ic && ib->l == 2) return  if_l_l_2(t, g, an, e);
    else if (!ic && ib->l > 2) return if_l_l_n(t, g, an, e);
    return atg_err(t, an, e, "atg if inv");
}

#define DFNES(t, T) static atg_stat dfn_##t##_e_##t##_s_##t(atg *t, gen *g, te *an, err **e) { \
    uint32_t vid = ast_lst_tbl_e_g_i(((te*) an->d[5].p)->d[3].p); \
    if (gen_a(g, GEN_OP(SET), gen_stkv(g, X64_TYPE(T), vid), gen_data(g, X64_TYPE(T), ((te*) an->d[6].p)->d[4]), NULL)) return atg_err(t, an, e, __FUNCTION__); \
    return ATG_STAT(OK); \
}

DFNES(i6, I6);
DFNES(u6, U6);

static atg_stat dfn_fn_e_fn_s__g(atg *t, gen *g, te *an, err **e) {
    (void) t;
    (void) g;
    (void) e;
    uint32_t lbl = ((te*) an->d[6].p)->d[4].u5;
    te *lte = ((te*) an->d[5].p)->d[3].p;
    ast_lst_tbl_e_s_i(lte, lbl);
    return ATG_STAT(OK);
}

static void vr_ab_nc(gen *g, size_t l, vr **v, un d) {
    if (!*v) *v = vr_i(l, g->va, (void*) te_f);
    vr_ab(v, d);
}

static atg_stat lst_args_var(atg *t, gen *g, err **e, lst *l, vr **v) {
    atg_stat stat = ATG_STAT(OK);
    uint32_t flgs;
    te *h = l->h, *an, *lte;
    while (h) {
        an = h->d[0].p;
        if ((stat = atg_r(t, g, an, e)) != ATG_STAT(OK)) return stat;
        switch (an->d[2].u4) {
            case AST_CLS(E):
                lte = an->d[3].p;
                flgs = ast_lst_tbl_e_g_f(lte);
                if (flgs & LTE_FLG(A)) vr_ab_nc(g, l->l, v, P(gen_arg(g, atg_x64_g_t(lte->d[2].p), ast_lst_tbl_e_g_i(lte))));
                else if (flgs & LTE_FLG(L)) vr_ab_nc(g, l->l, v, P(gen_stkv(g, atg_x64_g_t(lte->d[2].p), ast_lst_tbl_e_g_i(lte))));
                else return atg_err(t, an, e, "atg inv lst args FLG");
                break;
            case AST_CLS(S):
                vr_ab_nc(g, l->l, v, P(gen_data(g, atg_x64_g_t(an->d[3].p), an->d[4])));
                break;
            case AST_CLS(O):
            case AST_CLS(A):
                vr_ab_nc(g, l->l, v, P(te_c((atg_g_g(an))->d[1].p)));
                break;
            default:
                return atg_err(t, an, e, "atg lst arg cls inv");
        }
        h = h->d[2].p;
    }
    return stat;
}

static atg_stat call_npr(gen_op *go, const te *an) {
    bool npr = false;
    te *p = an->d[0].p;
    // TODO if parent op is ret
    if (p->d[2].u4 == AST_CLS(L)) { // if last stmt
        if (p->d[0].p && ((te*) p->d[0].p)->d[2].u4 == AST_CLS(R) && ((te*) p->d[0].p)->d[3].p) return ATG_STAT(OK); // if root and has exports
        lst *l = p->d[4].p;
        if (!l) return ATG_STAT(INV);
        if (an == l->t->d[0].p) npr = true;
    }
    if (!npr) return ATG_STAT(OK);
    switch (*go) {
        case GEN_OP(CALL):
            *go = GEN_OP(CALLNPR);
            break;
        case GEN_OP(CALLV):
            *go = GEN_OP(CALLVNPR);
            break;
        default:
            return ATG_STAT(INV);
    }
    return ATG_STAT(OK);
}

static atg_stat fn_call(atg *t, gen *g, te *restrict an, gen_op go, te *restrict ca) {
    uint32_t lbl = ast_lst_tbl_e_g_i(((te*) an->d[4].p)->d[3].p);
    x64_type xt = atg_x64_g_t(an->d[3].p);
    if (xt == X64_TYPE(N)) {
        if (gen_a(g, go, ca, gen_lbl(g, lbl), NULL) != GEN_STAT(OK)) return ATG_STAT(INV);
    } else if (gen_a(g, go, gen_tmp(g, xt, t->tc++), ca, gen_lbl(g, lbl)) != GEN_STAT(OK)) return ATG_STAT(INV);
    return ATG_STAT(OK);
}

static atg_stat aply_e_fn(atg *t, gen *g, te *an, err **e) {
    atg_stat stat = ATG_STAT(OK);
    vr *v = NULL;
    if ((stat = lst_args_var(t, g, e, an->d[5].p, &v)) != ATG_STAT(OK)) {
        vr_f(v);
        return stat;
    }
    gen_op go = GEN_OP(CALL);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    if ((stat = fn_call(t, g, an, go, gen_call_v(g, v))) != ATG_STAT(OK)) {
        vr_f(v);
        return atg_err(t, an, e, __FUNCTION__);
    }
    return stat;
}

static atg_stat aply_e_nf(atg *t, gen *g, te *an, err **e) {
    atg_stat stat = ATG_STAT(OK);
    uint32_t flgs, id;
    vr *v = NULL, *s = NULL;
    te *h = ((te*) ((te*) an->d[4].p)->d[3].p)->d[2].p, *ps, *pn, *lte, *kv;
    tbl *lt;
    if ((stat = lst_args_var(t, g, e, an->d[5].p, &v)) != ATG_STAT(OK)) {
        vr_f(v);
        return stat;
    }
    if (!h) return atg_err(t, an, e, "atg inv nf scope");
    h = ((tbl*) h->d[4].p)->i->h;
    while (h) {
        lte = h->d[0].p;
        ps = an;
        while (ps) {
            if (ast_g_pn(AST_CLS(L), ps, &pn) != AST_STAT(OK)) return atg_err(t, an, e, "atg cant get pn for scope var");
            lt = pn->d[3].p;
            if (tbl_g_i(lt, lte->d[0], &kv) == TBL_STAT(OK)) {
                flgs = ast_lst_tbl_e_g_f(kv);
                id = ast_lst_tbl_e_g_i(kv);
                if (flgs & LTE_FLG(L)) {
                    vr_ab_nc(g, lt->i->l, &s, P(gen_stkv(g, type_g_x64_type(kv->d[2].p), id)));
                } else return atg_err(t, an, e, "atg load scope var nyi");
                break;
            }
            ps = pn->d[0].p;
            if (ps->d[2].u4 == AST_CLS(O) && ps->d[4].u4 == OC(CST)) {
                pn = ps->d[5].p;
                if (pn->d[2].u4 == AST_CLS(T) && ((te*) pn->d[3].p)->d[1].u4 == TYPE(FN)) return atg_err(t, an, e, "atg cannot search for scope past fn");
            }
        }
        h = h->d[2].p;
    }
    gen_op go = GEN_OP(CALL);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    if ((stat = fn_call(t, g, an, go, gen_call_w(g, s, v))) != ATG_STAT(OK)) {
        vr_f(v);
        return atg_err(t, an, e, __FUNCTION__);
    }
    return stat;
}

static atg_stat oc_cond_gen(oc c, gen_op *go) {
    switch (c) {
        case OC(EQ):
            *go = GEN_OP(NE);
            break;
        case OC(GT):
            *go = GEN_OP(LTE);
            break;
        case OC(GTE):
            *go = GEN_OP(LT);
            break;
        case OC(LT):
            *go = GEN_OP(GTE);
            break;
        case OC(LTE):
            *go = GEN_OP(GT);
            break;
        default:
            return ATG_STAT(INV);
    }
    return ATG_STAT(OK);
}

static bool cond_valid_pn(te *an) {
    te *pn = an->d[0].p;
    if (pn->d[2].u4 == AST_CLS(L)) return true;
    if (pn->d[2].u4 == AST_CLS(O) && pn->d[4].u6 == OC(AND)) {
        pn = pn->d[0].p;
        if (pn->d[2].u4 == AST_CLS(L)) return true;
    }
    return false;
}

static atg_stat cond_e_s(atg *t, gen *g, te *an, err **e, x64_type xt) {
    // TODO check if or
    if (!cond_valid_pn(an)) return atg_err(t, an, e, "atg inv parent node for cond");
    gen_op go;
    if (oc_cond_gen(an->d[4].u6, &go) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv cond oc");
    if (gen_a(g, go, var_arg(g, ((te*) an->d[5].p)->d[3].p, xt), gen_data(g, xt, ((te*) an->d[6].p)->d[4]), gen_lbl(g, UINT32_MAX)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat cond_bl_e_i6_s_i6(atg *t, gen *g, te *an, err **e) {
    return cond_e_s(t, g, an, e, X64_TYPE(I6));
}

static atg_stat cond_bl_e_u6_s_u6(atg *t, gen *g, te *an, err **e) {
    return cond_e_s(t, g, an, e, X64_TYPE(U6));
}

const char *atg_dump_strs[TYPE(_END)] = {
    [TYPE(U6)] = "(U6 %lu)\n",
    [TYPE(I6)] = "(I6 %ld)\n",
    [TYPE(F6)] = "(F6 %lf)\n"
};

static atg_stat dump_vd(atg *t, gen *g, te *restrict an, err **e, te *restrict cd) {
    atg_stat stat = ATG_STAT(OK);
    (void) t;
    uint32_t fd = ((te*) an->d[5].p)->d[4].u5;
    if (fd == 0 || fd > 2) return atg_err(t, an, e, "atg inv dump fd");
    FILE *f = fd == 1 ? stdout : stderr;
    te *type;
    if (ast_g_t(an->d[6].p, &type) != AST_STAT(OK)) return atg_err(t, an, e, "atg ast_g_t");
    vr *v = vr_i(3, g->va, (void*) te_f);
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(f))));
    switch (type->d[1].u4) {
        case TYPE(U6):
            vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(U6)]))));
            break;
        case TYPE(I6):
            vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)]))));
            break;
        case TYPE(F6):
            vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(F6)]))));
            break;
        default:
            vr_f(v);
            return atg_err(t, an, e, "atg inv dump type");
    }
    vr_ab(&v, P(cd));
    gen_op go = GEN_OP(CALLV);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    if (gen_a(g, go, gen_call_v(g, v), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return stat;
}

static atg_stat dump_vd_s_u5_aply(atg *t, gen *g, te *an, err **e) {
    return dump_vd(t, g, an, e, te_c(atg_g_g(an->d[6].p)->d[1].p));
}

static atg_stat dump_vd_s_u5_e_i6(atg *t, gen *g, te *an, err **e) {
    return dump_vd(t, g, an, e, var_arg(g, ((te*) an->d[6].p)->d[3].p, X64_TYPE(I6)));
}

void atg_arith(atg *t);

atg *atg_b(atg *t) {
    atg_a_se(t, dfn_cst_type_lst_t, cst_type_lst_s, cst_type_lst_e);
    atg_a_se(t, root_lst_t, root_lst_s, root_lst_e);
    atg_a_a(t, TYPE(I6), AST_CLS(E), TYPE(FN), aply_e_fn);
    atg_a_a(t, TYPE(U6), AST_CLS(E), TYPE(FN), aply_e_fn);
    atg_a_a(t, TYPE(F6), AST_CLS(E), TYPE(FN), aply_e_fn);
    atg_a_a(t, TYPE(VD), AST_CLS(E), TYPE(NF), aply_e_nf);
    atg_a_o(t, OC(DFN), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), dfn_i6_e_i6_s_i6);
    atg_a_o(t, OC(DFN), TYPE(U6), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(U6), dfn_u6_e_u6_s_u6);
    atg_a_o(t, OC(DFN), TYPE(FN), AST_CLS(E), TYPE(FN), AST_CLS(S), TYPE(_G), dfn_fn_e_fn_s__g);
    atg_a_o(t, OC(DFN), TYPE(NF), AST_CLS(E), TYPE(NF), AST_CLS(S), TYPE(_G), dfn_fn_e_fn_s__g);
    atg_a_o(t, OC(CST), TYPE(FN), AST_CLS(T), TYPE(FN), AST_CLS(L), TYPE(_A), atg_ok);
    atg_a_o(t, OC(CST), TYPE(NF), AST_CLS(T), TYPE(NF), AST_CLS(L), TYPE(_A), atg_ok);
    atg_a_o(t, OC(CST), TYPE(F6), AST_CLS(T), TYPE(F6), AST_CLS(E), TYPE(U6), cst_f6_e_u6);
    atg_a_o(t, OC(LOOP), TYPE(VD), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A), loop_l_l);
    atg_a_o(t, OC(IF), TYPE(U6), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A), if_l_l);
    atg_a_o(t, OC(IF), TYPE(I6), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A), if_l_l);
    atg_a_o(t, OC(EQ), TYPE(BL), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(U6), cond_bl_e_u6_s_u6);
    atg_a_o(t, OC(GT), TYPE(BL), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), cond_bl_e_i6_s_i6);
    atg_a_o(t, OC(GT), TYPE(BL), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(U6), cond_bl_e_u6_s_u6);
    atg_a_o(t, OC(LT), TYPE(BL), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), cond_bl_e_i6_s_i6);
    atg_a_o(t, OC(LTE), TYPE(BL), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), cond_bl_e_i6_s_i6);
    atg_a_o(t, OC(AND), TYPE(BL), AST_CLS(O), TYPE(BL), AST_CLS(O), TYPE(BL), atg_ok);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(I6), dump_vd_s_u5_aply);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(U6), dump_vd_s_u5_aply);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(F6), dump_vd_s_u5_aply);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(I6), dump_vd_s_u5_e_i6);
    atg_arith(t);
    return t;
}

static err *__attribute__((noinline)) jit_run(const uint8_t *m, size_t ep, te *x) {
    return ((atg_jit*) &m[ep])(x);
}

err *atg_z(const alfr *ta, tbl *volatile et, const uint8_t *m, size_t ep) {
    err *e;
    te *x = NULL, *h, *d;
    if (et) x = te_i(et->i->l > 1 ? et->i->l : 2, ta, NULL);
    e = jit_run(m, ep, x);
    if (et) {
        h = et->i->h;
        while (h) {
            d = h->d[0].p;
            d->d[1] = x->d[opt_exp_eid(d)];
            h = h->d[2].p;
        }
        te_f(x);
    }
    return e;
}
