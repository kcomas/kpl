
#include "atg_x64.h"

void atg_d_p(const atg *t) {
    te *h = t->dt->i->h, *d;
    while (h) {
        d = h->d[0].p;
        type_p(d->d[0].p);
        printf(" %p\n", d->d[1].p);
        h = h->d[2].p;
    }
}

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
            case AST_CLS(V):
                printf("[%s %s]", ast_cls_str(hc), type_str(ht));
                break;
            case AST_CLS(O):
                printf("[%s %s]\n", ast_oc_str(hc), type_str(ht));
                atg_add_e_p(kv->d[1].p, 2, idnt + 1);
                break;
            case AST_CLS(A):
                printf("[%s %s]\n", ast_cls_str(hc), type_str(ht));
                atg_add_e_p(kv->d[1].p, 1, idnt + 1);
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

x64_type type_g_x64_type(const te *type) {
    if (!type || type->d[1].u4 >= TYPE(_END)) return X64_TYPE(N);
    static x64_type tm[TYPE(_END)] = {
        [TYPE(I3)] = X64_TYPE(I3),
        [TYPE(I4)] = X64_TYPE(I4),
        [TYPE(I5)] = X64_TYPE(I5),
        [TYPE(I6)] = X64_TYPE(I6),
        [TYPE(U3)] = X64_TYPE(U3),
        [TYPE(U5)] = X64_TYPE(U5),
        [TYPE(U6)] = X64_TYPE(U6),
        [TYPE(F5)] = X64_TYPE(F5),
        [TYPE(F6)] = X64_TYPE(F6),
        [TYPE(CS)] = X64_TYPE(S),
        [TYPE(SG)] = X64_TYPE(M),
        [TYPE(ER)] = X64_TYPE(M),
        [TYPE(TE)] = X64_TYPE(M),
        [TYPE(VR)] = X64_TYPE(M),
        [TYPE(ST)] = X64_TYPE(M),
        [TYPE(UN)] = X64_TYPE(M),
        [TYPE(CJ)] = X64_TYPE(M)
    };
    return tm[type->d[1].u4];
}

atg_stat atg_err(const atg *t, te *an, err **e, const char *m) {
    te_c(ast_g_root(an));
    *e = err_i(t->ea, ast_err_p, ast_err_f, an, m);
    return ATG_STAT(INV);
}

#define GC_M(N) GC_M##N

typedef enum {
    GC_M(N), // none
    GC_M(U), // unused
    GC_M(A) // all
} gc_m;

static atg_stat ref_vars(atg *t, gen *g, te *an, err **e, tbl *tt, gen_op go, gc_m gc) {
    atg_stat stat = ATG_STAT(OK);
    te *h = tt->i->h, *lte;
    un gcfn;
    while (h) {
        lte = h->d[0].p;
        uint32_t flgs = ast_lst_tbl_e_g_f(lte);
        if (!type_is_ref(((te*) lte->d[2].p)->d[1].u4) || (gc == GC_M(U) && (flgs & LTE_FLG(E))) || (flgs & LTE_FLG(A)) || (flgs & LTE_FLG(Y))) {
            h = h->d[2].p;
            continue;
        }
        if (gc) {
            switch (((te*) lte->d[2].p)->d[1].u4) {
                case TYPE(SG):
                    gcfn = P(mc_f);
                    break;
                case TYPE(TE):
                case TYPE(ST):
                case TYPE(UN):
                case TYPE(CJ):
                    gcfn = P(te_f);
                    break;
                case TYPE(VR):
                    gcfn = P(vr_f);
                    break;
                default:
                    return atg_err(t, an, e, "atg inv gc type");
            }
            if (gen_a(g, go, gen_call_m(g, 1, gen_stkv(g, X64_TYPE(M), ast_lst_tbl_e_g_i(lte))), gen_data(g, X64_TYPE(M), gcfn), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        } else if (gen_a(g, GEN_OP(SET), gen_stkv(g, X64_TYPE(M), ast_lst_tbl_e_g_i(lte)), gen_data(g, X64_TYPE(M), P(NULL)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, "atg inv zero ref");
        h = h->d[2].p;
    }
    return stat;
}

static atg_stat cst_type_lst_s(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    te *lte = ((te*) ((te*) an->d[0].p)->d[5].p)->d[3].p;
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, g->lbl = ast_lst_tbl_e_g_i(lte)), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if ((stat = ref_vars(t, g, an, e, ((te*) an->d[6].p)->d[3].p, GEN_OP(_END), GC_M(N))) != ATG_STAT(OK)) return stat;
    return ATG_STAT(OK);
}

static atg_stat cst_type_lst_e(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    gen_op go = GEN_OP(CALL);
    uint32_t ui;
    size_t eid;
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    if ((stat = ref_vars(t, g, an, e, ((te*) an->d[6].p)->d[3].p, go, GC_M(U))) != ATG_STAT(OK)) return stat;
    te *rt = ((te*) ((te*) an->d[5].p)->d[3].p)->d[2].p, *tal = ((lst*) ((te*) an->d[6].p)->d[4].p)->t->d[0].p, *ret = NULL, *kv;
    if (rt->d[1].u4 != TYPE(VD)) {
        switch (tal->d[2].u4) {
            case AST_CLS(S):
                return atg_err(t, an, e, "ret CLS S nyi");
                break;
            case AST_CLS(E):
                ret = var_arg(g, tal->d[3].p, type_g_x64_type(rt));
                break;
            case AST_CLS(Z):
                if (rt->d[1].u4 != TYPE(UN)) return atg_err(t, an, e, "atg inv sym ret");
                if (tbl_g_i(rt->d[2].p, tal->d[5], &kv) != TBL_STAT(OK)) return atg_err(t, an, e, "atg inv un ret");
                if (lst_g_i(((tbl*) rt->d[2].p)->i, P(kv), &eid) != LST_STAT(OK)) return atg_err(t, an, e, "atg inv idx for un ret");
                ui = t->tc++;
                kv = tal->d[4].p;
                if ((stat = atg_an_var(t, g, &kv, e)) != ATG_STAT(OK)) return stat;
                if ((stat = atg_te_init(t, g, an, e, rt, 2, ui)) != ATG_STAT(OK)) return stat;
                if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ui), gen_data(g, X64_TYPE(U3), U3(offsetof(te, d)))), gen_data(g, X64_TYPE(U6), U6(eid)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
                if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ui), gen_data(g, X64_TYPE(U3), U3(offsetof(te, d) + sizeof(void*)))), kv, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
                ret = gen_tmp(g, X64_TYPE(M), ui);
                break;
            default:
                ret = te_c(atg_g_g(tal)->d[1].p);
                break;
        }
        if (!ret) return atg_err(t, an, e, "atg inv ret for LEAVE");
        if (gen_a(g, GEN_OP(LEAVE), ret, NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    } else if (gen_a(g, GEN_OP(LEAVE), NULL, NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat root_lst_s(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, g->lbl = t->lc++), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if ((stat = ref_vars(t, g, an, e, an->d[3].p, GEN_OP(_END), GC_M(N))) != ATG_STAT(OK)) return stat;
    return ATG_STAT(OK);
}

#define ATG_EXP_ARG_ID 0

// calc offset to index into te
static uint32_t atg_te_d_c_o(uint32_t id) {
    return offsetof(te, d) + sizeof(void*) * id;
}

te *atg_te_idx_d(gen *g, uint32_t id) {
    uint32_t idx = atg_te_d_c_o(id);
    return gen_data(g, idx <= INT8_MAX ? X64_TYPE(U3) : X64_TYPE(U5), U5(idx));
}

// calc offset to index into vr
static uint32_t atg_vr_d_c_o(uint32_t id) {
    return offsetof(vr, d) + sizeof(void*) * id;
}

te *atg_vr_idx_d(gen *g, uint32_t id) {
    uint32_t idx = atg_vr_d_c_o(id);
    return gen_data(g, idx <= INT8_MAX ? X64_TYPE(U3) : X64_TYPE(U5), U5(idx));
}

static atg_stat root_lst_e(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    te *rn = an->d[0].p;
    gen_op go = GEN_OP(CALLNPR);
    if (rn->d[3].p) {
        go = GEN_OP(CALL);
        te *h = ((tbl*) rn->d[3].p)->i->h, *d, *es;
        while (h) {
            d = h->d[0].p;
            uint32_t id = opt_exp_id(d);
            uint16_t eid = opt_exp_eid(d), flgs = opt_exp_flgs(d);
            x64_type xt = type_g_x64_type(d->d[2].p);
            if (flgs & LTE_FLG(A)) es = gen_arg(g, xt, id);
            else if (flgs & LTE_FLG(L)) es = gen_stkv(g, xt, id);
            else if (flgs & LTE_FLG(F)) es = gen_data(g, X64_TYPE(U5), U5(id));
            else return atg_err(t, an, e, "atg inv exp type");
            if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_arg(g, X64_TYPE(M), ATG_EXP_ARG_ID), atg_te_idx_d(g, eid)), es, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
            h = h->d[2].p;
        }
    }
    if ((stat = ref_vars(t, g, an, e, an->d[3].p, go, GC_M(U))) != ATG_STAT(OK)) return stat;
    if (gen_a(g, GEN_OP(LEAVE), gen_data(g, X64_TYPE(M), P(NULL)), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

bool inloop(te *an) {
    while (an->d[2].u4 != AST_CLS(R)) {
        if (an->d[2].u4 == AST_CLS(O) && an->d[4].u4 == OC(LOOP)) return true;
        an = an->d[0].p;
    }
    return false;
}

atg_stat atg_g_un_ev(te *tn, uint64_t *esym, uint64_t *vsym) {
    te *h = ((tbl*) tn->d[2].p)->i->h;
    uint64_t i = 0, a = 0, b = 0;
    while (h) {
        mc *s = ((te*) h->d[0].p)->d[0].p;
        if (s->d[0] == 'e' && s->d[1] == '\0') a = i;
        else if (s->d[0] == 'v' && s->d[1] == '\0') {
            b = i;
            break;
        }
        h = h->d[2].p;
        i++;
    }
    if (b) {
        *esym = a;
        *vsym = b;
        return ATG_STAT(OK);
    }
    return ATG_STAT(INV);
}

te *var_arg(gen *g, te *lte, x64_type xt) {
    uint32_t id = ast_lst_tbl_e_g_i(lte);
    uint32_t flgs = ast_lst_tbl_e_g_f(lte);
    if (flgs & LTE_FLG(L)) return gen_stkv(g, xt, id);
    x64_type xmt = xt == X64_TYPE(M) ? X64_TYPE(MM) : xt;
    if (flgs & LTE_FLG(A)) return gen_arg(g, xmt, id);
    if (flgs & LTE_FLG(Y)) return gen_tmp(g, xmt, id);
    if ((xt = x64_type_to_ref(xt)) == X64_TYPE(N)) return NULL;
    if (flgs & LTE_FLG(S)) return gen_stka(g, xt, id);
    return NULL;
}

atg_stat atg_an_var(atg *t, gen *g, te **i, err **e) {
    atg_stat stat;
    te *x = *i;
    switch (x->d[2].u4) {
        case AST_CLS(S):
            *i = gen_data(g, type_g_x64_type(x->d[3].p), x->d[4]);
            break;
        case AST_CLS(E):
            x = x->d[3].p;
            *i = var_arg(g, x, type_g_x64_type(x->d[2].p));
            break;
        default:
            if ((stat = atg_r(t, g, x, e)) != ATG_STAT(OK)) return stat;
            *i = te_c(atg_g_g(x)->d[1].p);
            break;
    }
    return ATG_STAT(OK);
}

atg_stat atg_nop(atg *t, gen *g, te *an, err **e) {
    (void) t;
    (void) g;
    (void) an;
    (void) e;
    return ATG_STAT(OK);
}

atg_stat v_set_fn(atg *t, gen *g, te *restrict an, err **e, te *restrict n, uint32_t ti, uint32_t eid, atg_idx_d idxfn) {
    atg_stat stat;
    switch (n->d[2].u4) {
        case AST_CLS(S):
            if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ti), idxfn(g, eid++)), gen_data(g, type_g_x64_type(n->d[3].p), n->d[4]), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, "v_set_fn CLS S");
            break;
        case AST_CLS(E):
            if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ti), idxfn(g, eid++)), var_arg(g, n->d[3].p, type_g_x64_type(((te*) n->d[3].p)->d[2].p)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, "v_set_fn CLS E");
            break;
        case AST_CLS(V):
        case AST_CLS(O):
            if ((stat = atg_r(t, g, n, e)) != ATG_STAT(OK)) return stat;
            n = atg_g_g(n);
            if (!n) return atg_err(t, an, e, "atg inv g code");
            n = n->d[1].p;
            if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ti), idxfn(g, eid++)), te_c(n), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, "v_set_fn CLS O");
            break;
        case AST_CLS(Z):
            if ((stat = atg_r(t, g, n, e)) != ATG_STAT(OK)) return stat;
            return atg_err(t, an, e, "TODO");
            break;
        default:
            return atg_err(t, an, e, "atg inv vn lst cls");
    }
    return ATG_STAT(OK);
}

static atg_stat v_fn(atg *t, gen *g, te *an, err **e, const alfr *af, void *initfn, atg_idx_d idxfn) {
    atg_stat stat;
    te *kv, *h, *n;
    uint32_t ti = t->tc++, eid = 0;
    lst *l = an->d[4].p;
    if (!l) return atg_err(t, an, e, "atg inv v lst");
    if (tbl_g_i(t->dt, an->d[3], &kv) != TBL_STAT(OK)) return atg_err(t, an, e, "atg inv des");
    if (gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(M), ti), gen_call_m(g, 3, gen_data(g, X64_TYPE(U6), U6(l->l)), gen_data(g, X64_TYPE(M), P(af)), gen_data(g, X64_TYPE(M), kv->d[1])), gen_data(g, X64_TYPE(M), P(initfn))) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    h = l->h;
    while (h) {
        n = h->d[0].p;
        if ((stat = v_set_fn(t, g, an, e, n, ti, eid, idxfn)) != ATG_STAT(OK)) return stat;
        eid++;
        h = h->d[2].p;
    }
    if (gen_a(g, GEN_OP(NOP), gen_tmp(g, X64_TYPE(M), ti), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat v_te_fn(atg *t, gen *g, te *an, err **e) {
    lst *l = an->d[4].p;
    if (!l || l->l < 2) return atg_err(t, an, e, "atg inv te len");
    return v_fn(t, g, an, e, &al_te, te_i, atg_te_idx_d);
}

static atg_stat v_vr_fn(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    if ((stat = v_fn(t, g, an, e, &al_vr, vr_i, atg_vr_idx_d)) != ATG_STAT(OK)) return stat;
    te *v = ((te*) g->code->t->d[0].p)->d[1].p;
    if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, te_c(v), gen_data(g, X64_TYPE(U3), U3(offsetof(vr, l)))), gen_data(g, X64_TYPE(U6), U6(((lst*) an->d[4].p)->l)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(NOP), te_c(v), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat cst_f6_e_u6(atg *t, gen *g, te *an, err **e) {
    te *r = ((te*) an->d[6].p)->d[3].p;
    if (!(r = var_arg(g, r, X64_TYPE(U6)))) return atg_err(t, an->d[6].p, e, "atg cst r inv");
    if (gen_a(g, GEN_OP(CST), gen_tmp(g, X64_TYPE(F6), t->tc++), r, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat cst_t_o_t(atg *t, gen *g, te *an, err **e, x64_type xt) {
    te *r = atg_g_g(an->d[6].p)->d[1].p;
    if (gen_a(g, GEN_OP(CST), gen_tmp(g, xt, r->d[1].u5), te_c(r), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat cst_u6_o_i6(atg *t, gen *g, te *an, err **e) {
    return cst_t_o_t(t, g, an, e, X64_TYPE(U6));
}

static atg_stat cst_i6_z_u6(atg *t, gen *g, te *an, err **e) {
    return cst_t_o_t(t, g, an, e, X64_TYPE(I6));
}

gen_stat inc_ref_u(gen *g, x64_type xt, uint32_t ti) {
    if (xt != X64_TYPE(M)) return GEN_STAT(INV); // TODO MM
    te *tgt = gen_idx_m(g, X64_TYPE(I6), 2, gen_tmp(g, xt, ti), gen_data(g, X64_TYPE(U3), U3(0)));
    if (gen_a(g, GEN_OP(ADD), tgt, te_c(tgt), gen_data(g, X64_TYPE(I6), I6(1))) != GEN_STAT(OK)) return GEN_STAT(INV);
    return GEN_STAT(OK);
}

static atg_stat cst_cj_nf(atg *t, gen *g, te *an, err **e) {
    uint32_t ci = t->tc++, ni = t->tc++, idx = 0;
    te *lte, *en = NULL, *sn = an->d[6].p, *kv, *h, *n;
    tbl *nft = ((te*) ((te*) an->d[3].p)->d[2].p)->d[4].p;
    if (sn && sn->d[2].u4 == AST_CLS(E)) en = sn;
    else if (sn && sn->d[2].u4 == AST_CLS(O)) {
        sn = sn->d[5].p;
        if (sn && sn->d[2].u4 == AST_CLS(E)) en = sn;
    }
    if (!en) return atg_err(t, an, e, "atg inv entry for cj");
    if (tbl_g_i(t->dt, an->d[3], &kv) != TBL_STAT(OK)) return atg_err(t, an, e, "atg cannot get cj des");
    if (gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(M), ci), gen_call_m(g, 3, gen_data(g, X64_TYPE(U6), U6(nft->i->l + 1)), gen_data(g, X64_TYPE(M), P(&al_te)), gen_data(g, X64_TYPE(M), kv->d[1])), gen_data(g, X64_TYPE(M), P(te_i))) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    h = nft->i->h;
    while (h) {
        n = h->d[0].p;
        if (!(lte = chk_g_pn_lte(an, n->d[0].p))) return atg_err(t, an, e, "atg cannto get cj stkv");
        if (!type_eq(n->d[2].p, lte->d[2].p)) return atg_err(t, an, e, "atg inv stkv type for cj");
        x64_type xt = type_g_x64_type(n->d[2].p);
        if (gen_a(g, GEN_OP(SET), gen_tmp(g, xt, ni), var_arg(g, lte, xt), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, "atg inv stkv for cj");
        if (type_is_ref(((te*) n->d[2].p)->d[1].u4) && inc_ref_u(g, xt, ni) != GEN_STAT(OK)) return ATG_STAT(INV);
        if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ci), atg_te_idx_d(g, idx++)), gen_tmp(g, xt, ni), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        h = h->d[2].p;
    }
    lte = en->d[3].p;
    if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ci), atg_te_idx_d(g, idx)), gen_lbl(g, ast_lst_tbl_e_g_i(lte)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(NOP), gen_tmp(g, X64_TYPE(M), ci), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static gen_op atg_gen_g_cond_rv(const te *ci) {
    switch (ci->d[0].u6) {
        case GEN_OP(EQ): return GEN_OP(NE);
        case GEN_OP(NE): return GEN_OP(EQ);
        case GEN_OP(LT): return GEN_OP(GTE);
        case GEN_OP(LTE): return GEN_OP(GT);
        case GEN_OP(GT): return GEN_OP(LTE);
        case GEN_OP(GTE): return GEN_OP(LT);
        default:
            break;
    }
    return GEN_OP(_END);
}

static atg_stat cond_s_lbl(te *cnds, uint32_t sl, uint32_t el) {
    if (!cnds) return ATG_STAT(INV);
    while (cnds) {
        te *cnd = cnds->d[0].p, *lbl;
        if (gen_is_cond(cnd->d[0].u4)) {
            lbl = cnd->d[3].p;
            if (gen_var_g_c(lbl) == GEN_CLS(L)) {
                if (lbl->d[1].u5 == 0) lbl->d[1] = U5(sl);
                else if (lbl->d[1].u5 == UINT32_MAX) lbl->d[1] = U5(el);
            }
        }
        cnds = cnds->d[2].p;
    }
    return ATG_STAT(OK);
}

static atg_stat loop_l_l(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    uint32_t sl = t->lc++, el = t->lc++;
    te *cnds = g->code->t, *ende;
    if ((stat = atg_r(t, g, an->d[5].p, e)) != ATG_STAT(OK)) return stat;
    cnds = cnds->d[2].p; // gen code gen by cond
    if ((stat = cond_s_lbl(cnds, sl, el)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg loop cond not lbl");
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, sl), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if ((stat = atg_r(t, g, an->d[6].p, e)) != ATG_STAT(OK)) return stat;
    if ((stat = atg_r(t, g, an->d[5].p, e)) != ATG_STAT(OK)) return stat;
    ende = g->code->t->d[0].p;
    if (!gen_is_cond(ende->d[0].u4)) return atg_err(t, an, e, "atg loop ende not cond");
    ende->d[0] = U4(atg_gen_g_cond_rv(ende));
    if (gen_var_g_c(ende->d[3].p) != GEN_CLS(L)) return atg_err(t, an, e, "atg loop ende not lbl");
    ((te*) ende->d[3].p)->d[1] = U5(sl);
    if (gen_u_fn(g, ende) != GEN_STAT(OK)) return atg_err(t, an, e, "atg ci update inv");
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, el), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat if_cond(atg *t, gen *g, te *an, err **e, uint32_t tl) {
    atg_stat stat;
    te *h;
    x64_type xt;
    switch (an->d[2].u4) {
        case AST_CLS(S):
            xt = type_g_x64_type(an->d[3].p);
            if (gen_a(g, GEN_OP(SET), gen_tmp(g, xt, tl), gen_data(g, xt, an->d[4]), NULL)) return atg_err(t, an, e, __FUNCTION__);
            return ATG_STAT(OK);
        case AST_CLS(E):
            h = an->d[3].p;
            xt = type_g_x64_type(h->d[2].p);
            if (gen_a(g, GEN_OP(SET), gen_tmp(g, xt, tl), var_arg(g, h, xt), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
            return ATG_STAT(OK);
        case AST_CLS(L):
            h = ((lst*) an->d[4].p)->h;
            while (h) {
                if ((stat = if_cond(t, g, h->d[0].p, e, tl)) != ATG_STAT(OK)) return stat;
                h= h->d[2].p;
            }
            break;
        default:
            if ((stat = atg_r(t, g, an, e)) != ATG_STAT(OK)) return stat;
            break;
    }
    te *c = g->code->t->d[0].p;
    if (gen_var_g_c(c->d[1].p) == GEN_CLS(T)) {
        if (c->d[0].u6 != GEN_OP(NOP)) ((te*) c->d[1].p)->d[1] = U5(tl);
        else if (gen_a(g, GEN_OP(SET), gen_tmp(g, gen_var_g_t(c->d[1].p), tl), te_c(c->d[1].p), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    }
    return ATG_STAT(OK);
}

static atg_stat if_l_l_1(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    uint32_t sl = t->lc++, el = t->lc++;
    te *c = g->code->t;
    if ((stat = atg_r(t, g, an->d[5].p, e)) != ATG_STAT(OK)) return stat;
    c = c->d[2].p; // gen code gen by cond
    if ((stat = cond_s_lbl(c, sl, el)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg if cond not lbl");
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, sl), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if ((stat = if_cond(t, g, an->d[6].p, e, t->tc++)) != ATG_STAT(OK)) return stat;
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, el), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat if_l_l_2(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    uint32_t sl = t->lc++, ml = t->lc++, el = t->lc++, tl = t->tc++;
    te *c = g->code->t;
    if ((stat = atg_r(t, g, an->d[5].p, e)) != ATG_STAT(OK)) return stat;
    c = c->d[2].p; // gen code gen by cond
    if ((stat = cond_s_lbl(c, sl, ml)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg if cond not lbl");
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
        if ((stat = cond_s_lbl(c, sl, ml)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg if cond not lbl");
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
    te *tn = an->d[3].p;
    if (!tn) return atg_err(t, an, e, "atg inv if type");
    if (ic && ib->l == 1) return if_l_l_1(t, g, an, e);
    if (tn->d[1].u4 == TYPE(VD)) return atg_err(t, an, e, "atg if VD nyi");
    if (ic && ib->l == 2) return  if_l_l_2(t, g, an, e);
    if (!ic && ib->l > 2) return if_l_l_n(t, g, an, e);
    return atg_err(t, an, e, "atg if inv");
}

atg_stat atg_te_init(atg *t, gen *g, te *restrict an, err **e, te *restrict type, size_t l, uint32_t ti) {
    te *kv;
    if (tbl_g_i(t->dt, P(type), &kv) != TBL_STAT(OK)) return atg_err(t, an, e, "atg inv des");
    if (gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(M), ti), gen_call_m(g, 3, gen_data(g, X64_TYPE(U6), U6(l)), gen_data(g, X64_TYPE(M), P(&al_te)), gen_data(g, X64_TYPE(M), kv->d[1])), gen_data(g, X64_TYPE(M), P(te_i))) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat mtch_st(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    size_t eid;
    uint32_t ti = t->tc++;
    te *st = an->d[3].p, *h, *kv, *n;
    lst *sl = ((te*) an->d[6].p)->d[4].p;
    if (!sl) return atg_err(t, an, e, "atg inv st lst");
    h = sl->h;
    if ((stat = atg_te_init(t, g, an, e, st, sl->l, ti)) != ATG_STAT(OK)) return stat;
    while (h) {
        n = h->d[0].p;
        if (tbl_g_i(st->d[2].p, n->d[5], &kv) != TBL_STAT(OK)) return atg_err(t, an, e, "atg inv key for st tbl");
        if (lst_g_i(((tbl*) st->d[2].p)->i, P(kv), &eid) != LST_STAT(OK)) return atg_err(t, an, e, "atg inv idx for st tbl itm");
        if ((stat = v_set_fn(t, g, an, e, n->d[4].p, ti, (uint32_t) eid, atg_te_idx_d)) != ATG_STAT(OK)) return stat;
        h = h->d[2].p;
    }
    if (gen_a(g, GEN_OP(NOP), gen_tmp(g, X64_TYPE(M), ti), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static te *mtch_g_un(gen *g, te *r, x64_type xt) {
    te *rv;
    if (r->d[2].u4 == AST_CLS(E)) rv = var_arg(g, r->d[3].p, xt);
    else rv = te_c(atg_g_g(r)->d[1].p);
    return rv;
}

static atg_stat mtch_l_l(atg *t, gen *g, te *an, err **e) {
    if (!((te*) an->d[5].p)->d[4].p) return mtch_st(t, g, an, e);
    atg_stat stat;
    x64_type xt;
    size_t eid;
    uint32_t el = t->lc++, nl = 0, tl = t->tc++;
    te *l = an->d[6].p, *r = ((lst*) ((te*) an->d[5].p)->d[4].p)->h->d[0].p, *h, *n, *ut, *kv;
    if ((stat = atg_r(t, g, r, e)) != ATG_STAT(OK)) return stat;
    if (ast_g_t(r, &ut) != AST_STAT(OK)) return atg_err(t, an, e, "atg inv type for mtch");
    if (ut->d[1].u4 == TYPE(SL)) ut = ut->d[2].p;
    h = ((lst*) l->d[4].p)->h;
    while (h) {
        n = h->d[0].p;
        switch (n->d[2].u4) {
            case AST_CLS(Z):
                if (tbl_g_i(ut->d[2].p, n->d[5], &kv) != TBL_STAT(OK)) return atg_err(t, an, e, "atg inv key for un tbl");
                if (lst_g_i(((tbl*) ut->d[2].p)->i, P(kv), &eid) != LST_STAT(OK)) return atg_err(t, an, e, "atg inv idx for un tbl itm");
                if (gen_a(g, GEN_OP(NE), gen_idx_m(g, X64_TYPE(U6), 2, mtch_g_un(g, r, X64_TYPE(M)), atg_te_idx_d(g, 0)), gen_data(g, X64_TYPE(U6), U6(eid)), gen_lbl(g, h->d[2].p ? nl = t->lc++ : el)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
                if (tbl_g_i(l->d[3].p, n->d[5], &kv) == TBL_STAT(OK)) {
                    xt = type_g_x64_type(kv->d[2].p);
                    if (gen_a(g, xt == X64_TYPE(M) ? GEN_OP(REF) : GEN_OP(SET), var_arg(g, kv, xt), gen_idx_m(g, xt, 2, mtch_g_un(g, r, xt), atg_te_idx_d(g, 1)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
                }
                n = n->d[4].p;
                if ((stat = if_cond(t, g, n, e, tl)) != ATG_STAT(OK)) return stat;
                if (h->d[2].p) {
                    if (gen_a(g, GEN_OP(JMP), gen_lbl(g, el), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);

                    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, nl), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
                }
                break;
            case AST_CLS(A):
                return atg_err(t, an, e, "nyi");
                break;
            default:
                if ((stat = if_cond(t, g, n, e, tl)) != ATG_STAT(OK)) return stat;
                break;
        }
        h = h->d[2].p;
    }
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, el), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

atg_stat aply_te_g_idx(atg *t, te *an, err **e, size_t *n, te **lte) {
    lst *l = an->d[5].p;
    *lte = ((te*) an->d[4].p)->d[3].p;
    if (l->l != 1) return atg_err(t, an, e, "atg inv te idx");
    te *st = l->h->d[0].p;
    if (st->d[2].u4 != AST_CLS(S)) return atg_err(t, an, e, "atg te access cls inv");
    if (!st->d[3].p || ((te*) st->d[3].p)->d[1].u4 != TYPE(I6)) return atg_err(t, an, e, "atg te access type inv");
    *n = st->d[4].u6;
    if (!type_te_g_t((*lte)->d[2].p, *n)) return atg_err(t, an, e, "atg inv te access idx");
    return ATG_STAT(OK);
}

atg_stat z_lte_id(atg *t, te *an, err **e, const te *zn, te **lte, size_t *id) {
    te *kv;
    *lte = ((te*) zn->d[4].p)->d[3].p;
    tbl *st = ((te*) (*lte)->d[2].p)->d[2].p;
    if (tbl_g_i(st, zn->d[5], &kv) != TBL_STAT(OK)) return atg_err(t, an, e, "atg inv key for st tbl");
    if (lst_g_i(st->i, P(kv), id) != LST_STAT(OK)) return atg_err(t, an, e, "atg inv idx for st tbl itm");
    return ATG_STAT(OK);
}

atg_stat call_npr(gen_op *go, const te *an) {
    bool npr = false;
    te *p = an->d[0].p;
    // TODO if parent op is ret
    if (p && p->d[2].u4 == AST_CLS(L)) { // if last stmt
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

static atg_stat z_e_st(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    size_t eid;
    te *lte = an->d[0].p;
    if (lte && lte->d[2].u4 == AST_CLS(O) && lte->d[4].u4 == OC(AGN)) return ATG_STAT(OK);
    if ((stat = z_lte_id(t, an, e, an, &lte, &eid)) != ATG_STAT(OK)) return stat;
    x64_type xt = type_g_x64_type(((te*) an->d[3].p)->d[2].p);
    if (gen_a(g, GEN_OP(SET), gen_tmp(g, xt, t->tc++), gen_idx_m(g, xt, 2, var_arg(g, lte, X64_TYPE(M)), atg_te_idx_d(g, eid)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static te *fn_root_lst(te *an) {
    te *pn, *ppn;
    do {
        if (ast_g_pn(AST_CLS(L), an, &pn) != AST_STAT(OK)) NULL;
        an = ppn = pn->d[0].p;
        if (ppn->d[2].u4 == AST_CLS(R)) return pn;
        if (ppn->d[2].u4 == AST_CLS(O) && ppn->d[4].u4 == OC(CST)) {
            if (ast_g_t(ppn->d[5].p, &pn) != AST_STAT(OK)) return NULL;
            if (type_g_c(pn->d[1].u4) == TYPE_CLS(F)) return ppn->d[6].p;
        }
    } while (an);
    return NULL;
}

const char *atg_un_inv_str = "inv un";

void atg_rt_err_p(void *p) {
    te *t = p;
    uint32_t cno = t->d[1].u5;
    uint16_t lno = t->d[1].u6 >> 32;
    printf("\e[1m%d::%d ", lno, cno);
    mc *s = t->d[0].p;
    if (s) printf("%s", (char*) s->d);
    s = t->d[2].p;
    printf("\e[0m\n\e[91m`%s\e[0m", (char*) s->d);
}

void atg_rt_err_f(void *p) {
    te *t = p;
    mc_f(t->d[0].p);
    mc_f(t->d[2].p);
    t->af->f(t);
}

// te[filename;lineno|charno;msg]
gen_stat atg_rt_err_u(gen *g, uint32_t ri, uint32_t si, uint32_t ti, mc *fname, uint16_t lno, uint32_t cno, const char *msg) {
    gen_stat stat;
    if ((stat = gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(M), ri), gen_call_m(g, 3, gen_data(g, X64_TYPE(U6), U6(3)), gen_data(g, X64_TYPE(M), P(&al_te)), gen_data(g, X64_TYPE(M), P(atg_rt_err_f))), gen_data(g, X64_TYPE(M), P(te_i)))) != GEN_STAT(OK)) return stat;
    if (fname) {
        if ((stat = gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(M), si), gen_call_m(g, 2, gen_data(g, X64_TYPE(S), P(fname)), gen_data(g, X64_TYPE(M), P(&al_mc))), gen_data(g, X64_TYPE(M), P(mc_i_cstr)))) != GEN_STAT(OK)) return stat;
        if ((stat = gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ri), gen_data(g, X64_TYPE(U3), U3(offsetof(te, d)))), gen_tmp(g, X64_TYPE(M), si), NULL)) != GEN_STAT(OK)) return stat;
    }
    uint64_t lc = (uint64_t) lno << 32 | cno;
    if ((stat = gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ri), gen_data(g, X64_TYPE(U3), U3(offsetof(te, d) + sizeof(void*)))), gen_data(g, X64_TYPE(U6), U6(lc)), NULL) != GEN_STAT(OK))) return stat;
    if ((stat = gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ri), gen_data(g, X64_TYPE(U3), U3(offsetof(te, d) + sizeof(void*) * 2))), gen_tmp(g, X64_TYPE(M), ti), NULL) != GEN_STAT(OK))) return stat;
    return gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(M), ri), gen_call_m(g, 5, gen_data(g, X64_TYPE(M), P(&al_err)), gen_data(g, X64_TYPE(M), P(atg_rt_err_p)), gen_data(g, X64_TYPE(M), P(te_f)), gen_tmp(g, X64_TYPE(M), ri), gen_data(g, X64_TYPE(M), P(msg))), gen_data(g, X64_TYPE(M), P(err_i)));
}

static atg_stat atg_gc_scope(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    te *ln;
    if (!(ln = fn_root_lst(an))) return atg_err(t, an, e, "atg inv parent lst");
    if ((stat = ref_vars(t, g, an, e, ln->d[3].p, GEN_OP(CALL), GC_M(A))) != ATG_STAT(OK)) return stat;
    return ATG_STAT(OK);
}

static atg_stat un_inv_a(atg *t, gen *g, te *an, err **e, mc *s, uint32_t ri, uint32_t si, uint32_t ti) {
    atg_stat stat;
    if ((stat = atg_gc_scope(t, g, an, e)) != ATG_STAT(OK)) return stat;
    // TODO check if inside function
    te *tkn = ((te*) an->d[1].p)->d[2].p;
    if (gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(M), ti), gen_call_m(g, 2, gen_data(g, X64_TYPE(S), P(s)), gen_data(g, X64_TYPE(M), P(&al_mc))), gen_data(g, X64_TYPE(M), P(mc_i_cstr))) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (atg_rt_err_u(g, ri, si, ti, node_root_fname(an->d[1].p), tkn_m_g_l(tkn), tkn_m_g_c(tkn), atg_un_inv_str) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(M), ri), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat z_e_un_lte(atg *t, gen *g, te *restrict an, err **e, te *restrict lte) {
    atg_stat stat;
    uint32_t el = t->lc++, ri = t->tc++, si = t->tc++, ti = t->tc++;
    size_t eid = 0;
    te *ut, *kv;
    if (ast_g_t(an->d[4].p, &ut) != AST_STAT(OK)) return atg_err(t, an, e, "atg inv type for Z");
    if (tbl_g_i(ut->d[2].p, an->d[5], &kv) != TBL_STAT(OK)) return atg_err(t, an, e, "atg inv key for un tbl");
    if (lst_g_i(((tbl*) ut->d[2].p)->i, P(kv), &eid) != LST_STAT(OK)) return atg_err(t, an, e, "atg inv idx for un tbl itm");
    if (gen_a(g, GEN_OP(EQ), gen_idx_m(g, X64_TYPE(U6), 2, gen_stkv(g, X64_TYPE(M), ast_lst_tbl_e_g_i(lte)), atg_te_idx_d(g, 0)), gen_data(g, X64_TYPE(U6), U6(eid)), gen_lbl(g, el)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if ((stat = un_inv_a(t, g, an, e, kv->d[0].p, ri, si, ti)) != ATG_STAT(OK)) return stat;
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, el), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    x64_type xt = type_g_x64_type(((te*) an->d[3].p)->d[2].p);
    if (gen_a(g, GEN_OP(SET), gen_tmp(g, xt, t->tc++), gen_idx_m(g, xt, 2, gen_stkv(g, X64_TYPE(M), ast_lst_tbl_e_g_i(lte)), atg_te_idx_d(g, 1)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat z_e_un(atg *t, gen *g, te *an, err **e) {
    return z_e_un_lte(t, g, an, e, ((te*) an->d[4].p)->d[3].p);
}

static atg_stat z_e_un_o(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    if ((stat = atg_r(t, g, an->d[4].p, e)) != ATG_STAT(OK)) return stat;
    return z_e_un_lte(t, g, an, e, ((te*) ((te*) an->d[4].p)->d[5].p)->d[3].p);
}

static atg_stat z_un_(atg *t, gen *g, te *an, err **e) {
    (void) g;
    // TODO init un
    // TODO determine idx of VD
    // TODO set idx of VD
    // TODO nop un
    return atg_err(t, an, e, "TODO INIT VD UN");
}

static atg_stat oc_cond_gen(oc c, gen_op *go, bool rv) {
    switch (c) {
        case OC(EQ):
            *go = rv ? GEN_OP(NE) : GEN_OP(EQ);
            break;
        case OC(NE):
            *go = rv ? GEN_OP(EQ) : GEN_OP(NE);
            break;
        case OC(GT):
            *go = rv ? GEN_OP(LTE) : GEN_OP(GT);
            break;
        case OC(GTE):
            *go = rv ? GEN_OP(LT) : GEN_OP(GTE);
            break;
        case OC(LT):
            *go = rv ? GEN_OP(GTE) : GEN_OP(LT);
            break;
        case OC(LTE):
            *go = rv ? GEN_OP(GT) : GEN_OP(LTE);
            break;
        default:
            return ATG_STAT(INV);
    }
    return ATG_STAT(OK);
}

static atg_stat atg_and_or_pn(atg *t, gen *g, te *restrict an, err **e, te *restrict r, te *restrict l, gen_op go) {
    uint32_t ti;
    te *pn;
    if (ast_g_pn(AST_CLS(O), an->d[0].p, &pn) != AST_STAT(OK)) return ATG_STAT(OK);
    switch (pn->d[4].u4) {
        case OC(IF):
        case OC(MTCH):
        case OC(LOOP):
            return ATG_STAT(OK);
        default:
            break;
    }
    r = r->d[3].p;
    l = l->d[3].p;
    if (gen_var_g_c(r) != GEN_CLS(T) || gen_var_g_c(l) != GEN_CLS(T)) return atg_err(t, an, e, "atg inv nested cond");
    if (gen_a(g, go, gen_tmp(g, X64_TYPE(U3), ti = t->tc++), te_c(r), te_c(l)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(NOP), gen_tmp(g, X64_TYPE(U3), ti), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat atg_and(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    te *l, *r;
    if ((stat = atg_r(t, g, an->d[6].p, e)) != ATG_STAT(OK)) return stat;
    r = g->code->t->d[0].p;
    if (r->d[0].u4 == GEN_OP(NOP)) return atg_err(t, an, e, "TODO AND NOP");
    if ((stat = atg_r(t, g, an->d[5].p, e)) != ATG_STAT(OK)) return stat;
    l = g->code->t->d[0].p;
    if (l->d[0].u4 == GEN_OP(NOP)) return atg_err(t, an, e, "TODO AND NOP");
    return atg_and_or_pn(t, g, an, e, r, l, GEN_OP(AND));
}

static atg_stat atg_or(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    te *ct = g->code->t, *tcnd, *cnd, *r, *l;
    if ((stat = atg_r(t, g, an->d[6].p, e)) != ATG_STAT(OK)) return stat;
    tcnd = g->code->t;
    r = tcnd->d[0].p;
    if (r->d[0].u4 == GEN_OP(NOP)) {
        return atg_err(t, an, e, "TODO OR NOP");
    } else {
        while (tcnd != ct) {
            cnd = tcnd->d[0].p;
            if (gen_is_cond(cnd->d[0].u4)) {
                ((te*) cnd->d[3].p)->d[1] = U6(0);
                cnd->d[0] = U4(atg_gen_g_cond_rv(cnd));
                if (gen_u_fn(g, cnd) != GEN_STAT(OK)) return atg_err(t, an, e, "gen or update gen ci inv");
            }
            tcnd = tcnd->d[1].p;
        }
    }
    if ((stat = atg_r(t, g, an->d[5].p, e)) != ATG_STAT(OK)) return stat;
    l = g->code->t->d[0].p;
    if (l->d[0].u4 == GEN_OP(NOP) && gen_a(g, GEN_OP(EQ), te_c(l->d[1].p), gen_data(g, X64_TYPE(U3), U3(0)), gen_lbl(g, UINT32_MAX)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return atg_and_or_pn(t, g, an, e, r, l, GEN_OP(OR));
}

static atg_stat atg_cond_(atg *t, gen *g, te *restrict an, err **e, te *restrict l, te *restrict r) {
    bool rv = false, set = true;
    size_t n = 2;
    gen_op go;
    te *pn = an;
    while (n > 0) {
        if (ast_g_pn(AST_CLS(O), pn->d[0].p, &pn) != AST_STAT(OK)) return atg_err(t, an, e, "atg cond inv parent");
        switch (pn->d[4].u4) {
            case OC(IF):
            case OC(LOOP):
                rv = true;
                set = false;
                n = 0;
                break;
            case OC(AND):
            case OC(OR):
                n--;
                break;
            default:
                n = 0;
                break;
        }
    }
    if (oc_cond_gen(an->d[4].u6, &go, rv) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv cond oc");
    if (gen_a(g, go, l, r, set ? gen_tmp(g, X64_TYPE(U3), t->tc++) : gen_lbl(g, UINT32_MAX)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat cond_bl_e_e_(atg *t, gen *g, te *an, err **e) {
    te *l = ((te*) an->d[5].p)->d[3].p, *r = ((te*) an->d[6].p)->d[3].p;
    return atg_cond_(t, g, an, e, var_arg(g, l, type_g_x64_type(l->d[2].p)), var_arg(g, r, type_g_x64_type(r->d[2].p)));
}

static atg_stat cond_e_s(atg *t, gen *g, te *an, err **e, x64_type xt) {
    return atg_cond_(t, g, an, e, var_arg(g, ((te*) an->d[5].p)->d[3].p, xt), gen_data(g, xt, ((te*) an->d[6].p)->d[4]));
}

static atg_stat cond_bl_e_i6_s_i6(atg *t, gen *g, te *an, err **e) {
    return cond_e_s(t, g, an, e, X64_TYPE(I6));
}

static atg_stat cond_bl_a_i6_s_i6(atg *t, gen *g, te *an, err **e) {
    return atg_cond_(t, g, an, e, te_c(atg_g_g(an->d[5].p)->d[1].p), gen_data(g, X64_TYPE(I6), ((te*) an->d[6].p)->d[4]));
}

static atg_stat cond_bl_e_u6_s_u6(atg *t, gen *g, te *an, err **e) {
    return cond_e_s(t, g, an, e, X64_TYPE(U6));
}

static atg_stat cond_bl_s_i6_o_i6(atg *t, gen *g, te *an, err **e) {
    return atg_cond_(t, g, an, e, gen_data(g, X64_TYPE(I6), ((te*) an->d[5].p)->d[4]), te_c(atg_g_g(an->d[6].p)->d[1].p));
}

static atg_stat cond_bl_e_f6_s_f6(atg *t, gen *g, te *an, err **e) {
    return cond_e_s(t, g, an, e, X64_TYPE(F6));
}

static atg_stat atg_cs_sg(atg *t, gen *g, te *an, err **e) {
    if (gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(M), t->tc++), gen_call_m(g, 2, gen_data(g, X64_TYPE(S), ((te*) an->d[6].p)->d[4]), gen_data(g, X64_TYPE(M), P(&al_mc))), gen_data(g, X64_TYPE(M), P(mc_i_cstr))) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat atg_cnct_vr_e_vr_(atg *t, gen *g, te *an, err **e) {
    uint32_t vt = t->tc++, xt = t->tc++;
    te *vlte = ((te*) an->d[5].p)->d[3].p, *rc;
    rc = an->d[6].p;
    if (rc->d[2].u4 == AST_CLS(E)) {
        rc = rc->d[3].p;
        rc = var_arg(g, rc, type_g_x64_type(rc->d[2].p));
    } else {
        rc = atg_g_g(rc);
        if (!rc) return atg_err(t, an, e, "atg inv rc");
        rc = te_c(rc->d[1].p);
    }
    if (gen_a(g, GEN_OP(REF), gen_tmp(g, X64_TYPE(MM), vt), var_arg(g, vlte, X64_TYPE(M)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(M), xt), rc, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(CALL), gen_call_m(g, 2, gen_tmp(g, X64_TYPE(MM), vt), gen_tmp(g, X64_TYPE(M), xt)), gen_data(g, X64_TYPE(M), P(vr_ab)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(NOP), gen_tmp(g, X64_TYPE(MM), vt), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat atg_uner(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    uint32_t nlb = t->lc++, glb = t->lc++, ei = t->tc++, ri = t->tc++, si = t->tc++, ti = t->tc++, vi = t->tc++;
    uint64_t esym, vsym;
    te *rt = ((te*) an->d[6].p)->d[3].p, *r = atg_g_g(an->d[6].p)->d[1].p, *syme;
    if (atg_g_un_ev(rt, &esym, &vsym) != ATG_STAT(OK)) return atg_err(t, an, e, "atg uner inv un `e`v");
    if (gen_a(g, GEN_OP(EQ), gen_idx_m(g, X64_TYPE(U6), 2, te_c(r), atg_te_idx_d(g, 0)), gen_data(g, X64_TYPE(U6), U6(vsym)), gen_lbl(g, glb)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(NE), gen_idx_m(g, X64_TYPE(U6), 2, te_c(r), atg_te_idx_d(g, 0)), gen_data(g, X64_TYPE(U6), U6(esym)), gen_lbl(g, nlb)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if ((stat = atg_gc_scope(t, g, an, e)) != ATG_STAT(OK)) return stat;
    if (!chk_g_pn_fnnf_type(an)) {
        if (gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(M), ei), gen_idx_m(g, X64_TYPE(M), 2, te_c(r), atg_te_idx_d(g, 1)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        if (inc_ref_u(g, X64_TYPE(M), ei) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        if (gen_a(g, GEN_OP(CALL), gen_call_m(g, 1, te_c(r)), gen_data(g, X64_TYPE(M), P(te_f)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        if (gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(M), ei), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    } else if (gen_a(g, GEN_OP(LEAVE), te_c(r), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, nlb), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (!(syme = un_g_lte_ch(rt, 'v'))) return atg_err(t, an, e, "atg inv v in uner");
    if ((stat = un_inv_a(t, g, an, e, syme->d[0].p, ri, si, ti)) != ATG_STAT(OK)) return stat;
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, glb), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    x64_type xt = type_g_x64_type(syme->d[2].p);
    if (gen_a(g, GEN_OP(SET), gen_tmp(g, xt, vi), gen_idx_m(g, xt, 2, te_c(r), atg_te_idx_d(g, 1)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (type_is_ref(((te*) syme->d[2].p)->d[1].u4) && inc_ref_u(g, xt, vi) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(CALL), gen_call_m(g, 1, te_c(r)), gen_data(g, X64_TYPE(M), P(te_f)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(NOP), gen_tmp(g, xt, vi), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

void atg_dfn(atg *t);
void atg_arith(atg *t);
void atg_aply(atg *t);
void atg_dump(atg *t);

atg *atg_b(atg *t) {
    atg_a_se(t, dfn_cst_type_lst_t, cst_type_lst_s, cst_type_lst_e);
    atg_a_se(t, root_lst_t, root_lst_s, root_lst_e);
    atg_a_v(t, TYPE(TE), v_te_fn);
    atg_a_v(t, TYPE(VR), v_vr_fn);
    atg_a_z(t, TYPE(U6), AST_CLS(E), TYPE(ST), z_e_st);
    atg_a_z(t, TYPE(I6), AST_CLS(E), TYPE(ST), z_e_st);
    atg_a_z(t, TYPE(UN), AST_CLS(E), TYPE(ST), z_e_st);
    atg_a_z(t, TYPE(I6), AST_CLS(E), TYPE(UN), z_e_un);
    atg_a_z(t, TYPE(F6), AST_CLS(E), TYPE(UN), z_e_un);
    atg_a_z(t, TYPE(SG), AST_CLS(E), TYPE(UN), z_e_un);
    atg_a_z(t, TYPE(I6), AST_CLS(O), TYPE(UN), z_e_un_o);
    atg_a_z(t, TYPE(F6), AST_CLS(O), TYPE(UN), z_e_un_o);
    atg_a_z(t, TYPE(SG), AST_CLS(O), TYPE(UN), z_e_un_o);
    atg_a_z(t, TYPE(TE), AST_CLS(O), TYPE(UN), z_e_un_o);
    atg_a_z(t, TYPE(UN), AST_CLS(_), TYPE(_N), z_un_);
    atg_a_z(t, TYPE(VD), AST_CLS(S), TYPE(I6), atg_nop);
    atg_a_z(t, TYPE(VD), AST_CLS(S), TYPE(F6), atg_nop);
    atg_a_z(t, TYPE(VD), AST_CLS(O), TYPE(I6), atg_nop);
    atg_a_z(t, TYPE(VD), AST_CLS(O), TYPE(SG), atg_nop);
    atg_a_o(t, OC(NOP), TYPE(ET), AST_CLS(E), TYPE(ET), AST_CLS(S), TYPE(ET), atg_nop);
    atg_a_o(t, OC(CST), TYPE(FN), AST_CLS(T), TYPE(FN), AST_CLS(L), TYPE(_A), atg_nop);
    atg_a_o(t, OC(CST), TYPE(NF), AST_CLS(T), TYPE(NF), AST_CLS(L), TYPE(_A), atg_nop);
    atg_a_o(t, OC(CST), TYPE(F6), AST_CLS(T), TYPE(F6), AST_CLS(E), TYPE(U6), cst_f6_e_u6);
    atg_a_o(t, OC(CST), TYPE(U6), AST_CLS(T), TYPE(U6), AST_CLS(O), TYPE(I6), cst_u6_o_i6);
    atg_a_o(t, OC(CST), TYPE(I6), AST_CLS(T), TYPE(I6), AST_CLS(Z), TYPE(U6), cst_i6_z_u6);
    atg_a_o(t, OC(CST), TYPE(CJ), AST_CLS(T), TYPE(CJ), AST_CLS(O), TYPE(NF), cst_cj_nf);
    atg_a_o(t, OC(LOOP), TYPE(VD), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A), loop_l_l);
    atg_a_o(t, OC(IF), TYPE(U6), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A), if_l_l);
    atg_a_o(t, OC(IF), TYPE(I6), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A), if_l_l);
    atg_a_o(t, OC(IF), TYPE(VD), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A), if_l_l);
    atg_a_o(t, OC(MTCH), TYPE(SG), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A), mtch_l_l);
    atg_a_o(t, OC(MTCH), TYPE(ST), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A), mtch_l_l);
    atg_a_o(t, OC(MTCH), TYPE(VD), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A), mtch_l_l);
    atg_a_o(t, OC(EQ), TYPE(BL), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(U6), cond_bl_e_u6_s_u6);
    atg_a_o(t, OC(EQ), TYPE(BL), AST_CLS(S), TYPE(I6), AST_CLS(O), TYPE(I6), cond_bl_s_i6_o_i6);
    atg_a_o(t, OC(NE), TYPE(BL), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), cond_bl_e_i6_s_i6);
    atg_a_o(t, OC(NE), TYPE(BL), AST_CLS(S), TYPE(I6), AST_CLS(O), TYPE(I6), cond_bl_s_i6_o_i6);
    atg_a_o(t, OC(GT), TYPE(BL), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), cond_bl_e_i6_s_i6);
    atg_a_o(t, OC(GT), TYPE(BL), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(U6), cond_bl_e_u6_s_u6);
    atg_a_o(t, OC(LT), TYPE(BL), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), cond_bl_e_i6_s_i6);
    atg_a_o(t, OC(LT), TYPE(BL), AST_CLS(A), TYPE(I6), AST_CLS(S), TYPE(I6), cond_bl_a_i6_s_i6);
    atg_a_o(t, OC(LT), TYPE(BL), AST_CLS(E), TYPE(F6), AST_CLS(S), TYPE(F6), cond_bl_e_f6_s_f6);
    atg_a_o(t, OC(LTE), TYPE(BL), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), cond_bl_e_i6_s_i6);
    atg_a_o(t, OC(LTE), TYPE(BL), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6), cond_bl_e_e_);
    atg_a_o(t, OC(AND), TYPE(BL), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A), atg_and);
    atg_a_o(t, OC(OR), TYPE(BL), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A), atg_or);
    atg_a_o(t, OC(CSG), TYPE(SG), AST_CLS(_), TYPE(_N), AST_CLS(S), TYPE(CS), atg_cs_sg);
    atg_a_o(t, OC(CNCTA), TYPE(VR), AST_CLS(E), TYPE(VR), AST_CLS(O), TYPE(I6), atg_cnct_vr_e_vr_);
    atg_a_o(t, OC(CNCTA), TYPE(VR), AST_CLS(E), TYPE(VR), AST_CLS(E), TYPE(I6), atg_cnct_vr_e_vr_);
    atg_a_o(t, OC(CNCTA), TYPE(VR), AST_CLS(E), TYPE(VR), AST_CLS(O), TYPE(F6), atg_cnct_vr_e_vr_);
    atg_a_o(t, OC(UNER), TYPE(I6), AST_CLS(_), TYPE(_N), AST_CLS(A), TYPE(UN), atg_uner);
    atg_dfn(t);
    atg_arith(t);
    atg_aply(t);
    atg_dump(t);
    return t;
}

static err *__attribute__((noinline)) jit_run(const uint8_t *m, size_t ep, te *x) {
    return ((atg_jit*) &m[ep])(x);
}

err *atg_z(const atg *t, tbl *volatile et, const uint8_t *m, size_t ep) {
    te *x = NULL, *h, *d;
    if (et) x = te_i(et->i->l > 1 ? et->i->l : 2, t->ta, NULL);
    err *e = jit_run(m, ep, x);
    if (et) {
        h = et->i->h;
        while (h) {
            d = h->d[0].p;
            switch (((te*) d->d[2].p)->d[1].u4) {
                case TYPE(FN):
                    if ((ssize_t) (d->d[1].u6 = as_lbl_g_c_i(t->a, x->d[opt_exp_eid(d)].u5)) == -1) return err_i(t->ea, NULL, NULL, NULL, "atg fn idx inv");
                    break;
                default:
                    d->d[1] = x->d[opt_exp_eid(d)];
                    break;
            }
            h = h->d[2].p;
        }
        te_f(x);
    }
    return e;
}
