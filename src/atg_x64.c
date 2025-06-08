
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

static atg_stat atg_err(const atg *t, te *an, err **e, const char *m) {
    *e = err_i(t->ea, ast_err_p, (void*) te_f, te_c(an), m);
    return ATG_STAT(INV);
}

static atg_stat cst_type_lst_s(atg *t, gen *g, te *an, err **e) {
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, g->lbl = t->lc++), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    // TODO zero ref counted vars
    return ATG_STAT(OK);
}

static atg_stat cst_type_lst_e(atg *t, gen *g, te *an, err **e) {
    // TODO gc ref counted vars
    te *rt = ((te*) ((te*) an->d[5].p)->d[3].p)->d[2].p;
    if (rt->d[1].u4 != TYPE(VD)) {
        if (gen_a(g, GEN_OP(LEAVE), te_c(((te*) g->code->t->d[0].p)->d[1].p), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    } else if (gen_a(g, GEN_OP(LEAVE), NULL, NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat root_lst_s(atg *t, gen *g, te *an, err **e) {
    return cst_type_lst_s(t, g, an, e);
}

static atg_stat root_lst_e(atg *t, gen *g, te *an, err **e) {
    // TODO exports
    if (gen_a(g, GEN_OP(LEAVE), NULL, NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static te *var_arg(gen *g, te *lte, x64_type xt) {
    uint32_t id = ast_lst_tbl_e_g_i(lte);
    uint32_t flgs = ast_lst_tbl_e_g_f(lte);
    if (flgs & LTE_FLG(A)) return gen_arg(g, xt, id);
    // TODO var
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

static atg_stat loop_l_l(atg *t, gen *g, te *an, err **e) {
    (void) g;
    //uint32_t sl = t->lc++, el = t->lc++;
    return atg_err(t, an, e, "TODO");
}

static atg_stat op_tel_ter_t(atg *t, gen *g, te *an, err **e, gen_op oc, x64_type xt, const char *pf) {
    te *l = ((te*) an->d[5].p)->d[3].p, *r = ((te*) an->d[6].p)->d[3].p;
    if (!(l = var_arg(g, l, xt))) return atg_err(t, an->d[5].p, e, "atg op l inv");
    if (!(r = var_arg(g, r, xt))) return atg_err(t, an->d[6].p, e, "atg op r inv");
    if (gen_a(g, oc, gen_tmp(g, xt, t->tc++), l, r) != GEN_STAT(OK)) return atg_err(t, an, e, pf);
    return ATG_STAT(OK);
}

static atg_stat add_i6_e_i6_e_i6(atg *t, gen *g, te *an, err **e) {
   return op_tel_ter_t(t, g, an, e, GEN_OP(ADD), X64_TYPE(I6), __FUNCTION__);
}

static atg_stat mul_i6_e_i6_e_i6(atg *t, gen *g, te *an, err **e) {
   return op_tel_ter_t(t, g, an, e, GEN_OP(MUL), X64_TYPE(I6), __FUNCTION__);
}

static atg_stat add_i6_e_i6_o_i6(atg *t, gen *g, te *an, err **e) {
    te *l = ((te*) an->d[5].p)->d[3].p, *r = ((te*) g->code->t->d[0].p)->d[1].p;
    if (!(l = var_arg(g, l, X64_TYPE(I6)))) return atg_err(t, an->d[5].p, e, "atg op l inv");
    if (gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(I6), t->tc++), l, te_c(r)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat neg_i6_o_i6(atg *t, gen *g, te *an, err **e) {
    uint32_t id = ((te*) ((te*) g->code->t->d[0].p)->d[1].p)->d[1].u5;
    if (gen_a(g, GEN_OP(NEG), gen_tmp(g, X64_TYPE(I6), id), gen_tmp(g, X64_TYPE(I6), id), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat mul_f6_e_f6_e_f6(atg *t, gen *g, te *an, err **e) {
   return op_tel_ter_t(t, g, an, e, GEN_OP(MUL), X64_TYPE(F6), __FUNCTION__);
}

static atg_stat div_f6_o_f6_o_f6(atg *t, gen *g, te *an, err **e) {
    te *l = g->code->t->d[1].p;
    l = l->d[0].p;
    l = l->d[1].p;
    te *r = g->code->t->d[0].p;
    r = r->d[1].p;
    if (gen_a(g, GEN_OP(DIV), gen_tmp(g, X64_TYPE(F6), t->tc++), te_c(l), te_c(r)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat dfn_i6_e_i6_s_i6(atg *t, gen *g, te *an, err **e) {
    uint32_t vid = ast_lst_tbl_e_g_i(((te*) an->d[5].p)->d[3].p);
    if (gen_a(g, GEN_OP(SET), gen_stkv(g, X64_TYPE(I6), vid), gen_data(g, X64_TYPE(I6), ((te*) an->d[6].p)->d[4]), NULL)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat dfn_fn_e_fn_s__g(atg *t, gen *g, te *an, err **e) {
    (void) t;
    (void) g;
    (void) e;
    uint32_t lbl = ((te*) an->d[6].p)->d[4].u5;
    te *lte = ((te*) an->d[5].p)->d[3].p;
    ast_lst_tbl_e_s_i(lte, lbl);
    return ATG_STAT(OK);
}

static atg_stat lst_args_var(atg *t, gen *g, err **e, lst *l, vr **v) {
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

static atg_stat aply_e_fn(atg *t, gen *g, te *an, err **e) {
    atg_stat stat = ATG_STAT(OK);
    uint32_t lbl = ast_lst_tbl_e_g_i(((te*) an->d[4].p)->d[3].p);
    vr *v;
    if ((stat = lst_args_var(t, g, e, an->d[5].p, &v)) != ATG_STAT(OK)) {
        vr_f(v);
        return stat;
    }
    gen_op go = GEN_OP(CALL);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    if (gen_a(g, go, gen_tmp(g, atg_x64_g_t(an->d[3].p), t->tc++), gen_call_v(g, v), gen_lbl(g, lbl)) != GEN_STAT(OK)) {
        vr_f(v);
        return atg_err(t, an, e, __FUNCTION__);
    }
    return stat;
}

const char *atg_dump_strs[TYPE(_END)] = {
    [TYPE(I6)] = "(I6 %ld)\n",
    [TYPE(F6)] = "(F6 %lf)\n"
};

static atg_stat dump_vd_s_u5_aply(atg *t, gen *g, te *an, err **e) {
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
    vr_ab(&v, P(te_c(((te*) g->code->t->d[0].p)->d[1].p)));
    gen_op go = GEN_OP(CALLV);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    if (gen_a(g, go, gen_call_v(g, v), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return stat;
}

atg *atg_b(atg *t) {
    atg_a_se(t, cst_type_lst_t, cst_type_lst_s, cst_type_lst_e);
    atg_a_se(t, root_lst_t, root_lst_s, root_lst_e);
    atg_a_a(t, TYPE(I6), AST_CLS(E), TYPE(FN), aply_e_fn);
    atg_a_a(t, TYPE(F6), AST_CLS(E), TYPE(FN), aply_e_fn);
    atg_a_o(t, OC(DFN), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), dfn_i6_e_i6_s_i6);
    atg_a_o(t, OC(DFN), TYPE(FN), AST_CLS(E), TYPE(FN), AST_CLS(S), TYPE(_G), dfn_fn_e_fn_s__g);
    atg_a_o(t, OC(CST), TYPE(FN), AST_CLS(T), TYPE(FN), AST_CLS(L), TYPE(_A), atg_ok);
    atg_a_o(t, OC(CST), TYPE(F6), AST_CLS(T), TYPE(F6), AST_CLS(E), TYPE(U6), cst_f6_e_u6);
    atg_a_o(t, OC(LOOP), TYPE(VD), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A), loop_l_l);
    atg_a_o(t, OC(ADD), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6), add_i6_e_i6_e_i6);
    atg_a_o(t, OC(ADD), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(O), TYPE(I6), add_i6_e_i6_o_i6);
    atg_a_o(t, OC(SUB), TYPE(I6), AST_CLS(_), TYPE(_N), AST_CLS(O), TYPE(I6), neg_i6_o_i6);
    atg_a_o(t, OC(MUL), TYPE(F6), AST_CLS(E), TYPE(F6), AST_CLS(E), TYPE(F6), mul_f6_e_f6_e_f6);
    atg_a_o(t, OC(MUL), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6), mul_i6_e_i6_e_i6);
    atg_a_o(t, OC(DIV), TYPE(F6), AST_CLS(O), TYPE(F6), AST_CLS(O), TYPE(F6), div_f6_o_f6_o_f6);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(I6), dump_vd_s_u5_aply);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(F6), dump_vd_s_u5_aply);
    return t;
}
