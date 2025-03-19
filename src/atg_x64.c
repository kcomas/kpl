
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

static bool cst_type_lst_t(const te *an) {
    return an->d[2].u4 == AST_CLS(O) && an->d[4].u4 == OC(CST) && an->d[6].p && ((te*) an->d[6].p)->d[2].u4 == AST_CLS(L);
}

static bool root_lst_t(const te *an) {
    return an->d[2].u4 == AST_CLS(L) && an->d[0].p && ((te*) an->d[0].p)->d[2].u4 == AST_CLS(R);
}

bool atg_x64_enq(const te *an) {
    switch (an->d[2].u4) {
        case AST_CLS(O):
            return cst_type_lst_t(an);
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

atg_stat atg_err(atg_stat stat, te *an, te **e) {
    *e = te_c(an);
    return stat;
}

static atg_stat cst_type_lst_s(atg *t, gen *g, te *an, te **e) {
    (void) t;
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, g->lbl = t->lc++), NULL, NULL) != GEN_STAT(OK)) return atg_err(ATG_STAT(INV), an, e);
    if (gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL) != GEN_STAT(OK)) return atg_err(ATG_STAT(INV), an, e);
    // TODO zero ref counted vars
    return ATG_STAT(OK);
}

static atg_stat cst_type_lst_e(atg *t, gen *g, te *an, te **e) {
    (void) t;
    // TODO gc ref counted vars
    te *rt = ((te*) ((te*) an->d[5].p)->d[3].p)->d[2].p;
    if (rt->d[1].u4 != TYPE(VD)) {
        if (gen_a(g, GEN_OP(LEAVE), te_c(((te*) g->code->t->d[0].p)->d[1].p), NULL, NULL) != GEN_STAT(OK)) return atg_err(ATG_STAT(INV), an, e);
    } else if (gen_a(g, GEN_OP(LEAVE), NULL, NULL, NULL) != GEN_STAT(OK)) return atg_err(ATG_STAT(INV), an, e);
    return ATG_STAT(OK);
}

static atg_stat root_lst_s(atg *t, gen *g, te *an, te **e) {
    return cst_type_lst_s(t, g, an, e);
}

static atg_stat root_lst_e(atg *t, gen *g, te *an, te **e) {
    (void) t;
    if (gen_a(g, GEN_OP(LEAVE), NULL, NULL, NULL) != GEN_STAT(OK)) return atg_err(ATG_STAT(INV), an, e);
    return ATG_STAT(OK);
}

static te *var_arg(gen *g, te *lte, x64_type xt) {
    uint32_t id = ast_lst_tbl_e_g_i(lte);
    uint32_t flgs = ast_lst_tbl_e_g_f(lte);
    if (flgs & LTE_FLG(A)) return gen_arg(g, xt, id);
    // TODO var
    return NULL;
}

static atg_stat atg_ok(atg *t, gen *g, te *an, te **e) {
    (void) t;
    (void) g;
    (void) an;
    (void) e;
    return ATG_STAT(OK);
}

static atg_stat add_i6_e_i6_e_i6(atg *t, gen *g, te *an, te **e) {
    te *l = ((te*) an->d[5].p)->d[3].p, *r = ((te*) an->d[6].p)->d[3].p;
    if (!(l = var_arg(g, l, X64_TYPE(I6)))) return atg_err(ATG_STAT(INV), an->d[5].p, e);
    if (!(r = var_arg(g, r, X64_TYPE(I6)))) return atg_err(ATG_STAT(INV), an->d[6].p, e);
    if (gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(I6), t->tc++), l, r) != GEN_STAT(OK)) return atg_err(ATG_STAT(INV), an, e);
    return ATG_STAT(OK);
}

static atg_stat add_i6_e_i6_o_i6(atg *t, gen *g, te *an, te **e) {
    te *l = ((te*) an->d[5].p)->d[3].p, *r = ((te*) g->code->t->d[0].p)->d[1].p;
    if (!(l = var_arg(g, l, X64_TYPE(I6)))) return atg_err(ATG_STAT(INV), an->d[5].p, e);
    if (gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(I6), t->tc++), l, te_c(r)) != GEN_STAT(OK)) return atg_err(ATG_STAT(INV), an, e);
    return ATG_STAT(OK);
}

static atg_stat neg_i6_o_i6(atg *t, gen *g, te *an, te **e) {
    (void) t;
    uint32_t id = ((te*) ((te*) g->code->t->d[0].p)->d[1].p)->d[1].u5;
    if (gen_a(g, GEN_OP(NEG), gen_tmp(g, X64_TYPE(I6), id), gen_tmp(g, X64_TYPE(I6), id), NULL) != GEN_STAT(OK)) return atg_err(ATG_STAT(INV), an, e);
    return ATG_STAT(OK);
}

static atg_stat dfn_fn_e_fn_s__g(atg *t, gen *g, te *an, te **e) {
    (void) t;
    (void) g;
    (void) e;
    uint32_t lbl = ((te*) an->d[6].p)->d[4].u5;
    te *lte = ((te*) an->d[5].p)->d[3].p;
    ast_lst_tbl_e_s_i(lte, lbl);
    return ATG_STAT(OK);
}

static atg_stat lst_args_var(atg *t, gen *g, te **e, lst *l, vr **v) {
    atg_stat stat = ATG_STAT(OK);
    *v = vr_i(l->l, g->va, (void*) te_f);
    te *h = l->h;
    while (h) {
        te *an = h->d[0].p;
        if ((stat = atg_r(t, g, an, e)) != ATG_STAT(OK)) return stat;
        switch (an->d[2].u4) {
            case AST_CLS(S):
                vr_ab(v, P(gen_data(g, atg_x64_g_t(an->d[3].p), an->d[4])));
                break;
            default:
                return atg_err(ATG_STAT(INV), an, e);
        }
        h = h->d[2].p;
    }
    return stat;
}

static atg_stat call_npr(gen_op *go, const te *an) {
    bool npr = false;
    te *p = an->d[0].p;
    // TODO check if parent op is ret
    if (p->d[2].u4 == AST_CLS(L)) { // check last stmt
        lst *l = p->d[4].p;
        if (!l) return ATG_STAT(INV);
        te *t = l->t;
        if (an == t->d[0].p) npr = true;
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

static atg_stat aply_e_fn(atg *t, gen *g, te *an, te **e) {
    atg_stat stat = ATG_STAT(OK);
    uint32_t lbl = ((te*) an->d[4].p)->d[4].u5;
    vr *v;
    if ((stat = lst_args_var(t, g, e, an->d[5].p, &v)) != ATG_STAT(OK)) {
        vr_f(v);
        return atg_err(stat, an, e);
    }
    gen_op go = GEN_OP(CALL);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(stat, an, e);
    if (gen_a(g, go, gen_tmp(g, atg_x64_g_t(an->d[3].p), t->tc++), gen_call_v(g, v), gen_lbl(g, lbl)) != GEN_STAT(OK)) {
        vr_f(v);
        return atg_err(ATG_STAT(INV), an, e);
    }
    return stat;
}

static atg_stat dump_vd_s_u5_aply(atg *t, gen *g, te *an, te **e) {
    atg_stat stat = ATG_STAT(OK);
    (void) t;
    uint32_t fd = ((te*) an->d[5].p)->d[4].u5;
    if (fd == 0 || fd > 2) return atg_err(ATG_STAT(INV), an, e);
    FILE *f = fd == 1 ? stdout : stderr;
    te *type;
    if (ast_g_t(an->d[6].p, &type) != AST_STAT(OK)) return atg_err(ATG_STAT(INV), an, e);
    vr *v = vr_i(3, g->va, (void*) te_f);
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(f))));
    switch (type->d[1].u4) {
        case TYPE(I6):
            vr_ab(&v, P(gen_data(g, X64_TYPE(M), P("(I6 %ld)\n"))));
            vr_ab(&v, P(te_c(((te*) g->code->t->d[0].p)->d[1].p)));
            break;
        default:
            vr_f(v);
            return atg_err(ATG_STAT(INV), an, e);
    }
    gen_op go = GEN_OP(CALLV);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(ATG_STAT(INV), an, e);
    if (gen_a(g, go, gen_call_v(g, v), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(ATG_STAT(INV), an, e);
    return stat;
}

atg *atg_b(atg *t) {
    atg_a_se(t, cst_type_lst_t, cst_type_lst_s, cst_type_lst_e);
    atg_a_se(t, root_lst_t, root_lst_s, root_lst_e);
    atg_a_a(t, TYPE(I6), AST_CLS(E), TYPE(FN), aply_e_fn);
    atg_a_o(t, OC(DFN), TYPE(FN), AST_CLS(E), TYPE(FN), AST_CLS(S), TYPE(_G), dfn_fn_e_fn_s__g);
    atg_a_o(t, OC(CST), TYPE(FN), AST_CLS(T), TYPE(FN), AST_CLS(L), TYPE(_A), atg_ok);
    atg_a_o(t, OC(ADD), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6), add_i6_e_i6_e_i6);
    atg_a_o(t, OC(ADD), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(O), TYPE(I6), add_i6_e_i6_o_i6);
    atg_a_o(t, OC(SUB), TYPE(I6), AST_CLS(_), TYPE(_N), AST_CLS(O), TYPE(I6), neg_i6_o_i6);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(I6), dump_vd_s_u5_aply);
    return t;
}
