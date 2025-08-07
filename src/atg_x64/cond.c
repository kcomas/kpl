
#include "../atg_x64.h"

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

static atg_stat cond_bl_e_u6_o_u6(atg *t, gen *g, te *an, err **e) {
    return atg_cond_(t, g, an, e, var_arg(g, ((te*) an->d[5].p)->d[3].p, X64_TYPE(U6)), te_c(atg_g_g(an->d[6].p)->d[1].p));
}

static atg_stat cond_bl_a_i6_s_i6(atg *t, gen *g, te *an, err **e) {
    return atg_cond_(t, g, an, e, te_c(atg_g_g(an->d[5].p)->d[1].p), gen_data(g, X64_TYPE(I6), ((te*) an->d[6].p)->d[4]));
}

static atg_stat cond_bl_e_u6_s_u6(atg *t, gen *g, te *an, err **e) {
    return cond_e_s(t, g, an, e, X64_TYPE(U6));
}

static atg_stat cond_bl_o_u6_s_u6(atg *t, gen *g, te *an, err **e) {
    return atg_cond_(t, g, an, e, te_c(atg_g_g(an->d[5].p)->d[1].p), gen_data(g, X64_TYPE(U6), ((te*) an->d[6].p)->d[4]));
}

static atg_stat cond_bl_s_i6_o_i6(atg *t, gen *g, te *an, err **e) {
    return atg_cond_(t, g, an, e, gen_data(g, X64_TYPE(I6), ((te*) an->d[5].p)->d[4]), te_c(atg_g_g(an->d[6].p)->d[1].p));
}

static atg_stat cond_bl_e_f6_s_f6(atg *t, gen *g, te *an, err **e) {
    return cond_e_s(t, g, an, e, X64_TYPE(F6));
}

void atg_cond(atg *t) {
    atg_a_o(t, OC(EQ), TYPE(BL), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(U6), cond_bl_e_u6_s_u6);
    atg_a_o(t, OC(EQ), TYPE(BL), AST_CLS(O), TYPE(U6), AST_CLS(S), TYPE(U6), cond_bl_o_u6_s_u6);
    atg_a_o(t, OC(EQ), TYPE(BL), AST_CLS(S), TYPE(I6), AST_CLS(O), TYPE(I6), cond_bl_s_i6_o_i6);
    atg_a_o(t, OC(NE), TYPE(BL), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), cond_bl_e_i6_s_i6);
    atg_a_o(t, OC(NE), TYPE(BL), AST_CLS(S), TYPE(I6), AST_CLS(O), TYPE(I6), cond_bl_s_i6_o_i6);
    atg_a_o(t, OC(NE), TYPE(BL), AST_CLS(E), TYPE(U6), AST_CLS(O), TYPE(U6), cond_bl_e_u6_o_u6);
    atg_a_o(t, OC(GT), TYPE(BL), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), cond_bl_e_i6_s_i6);
    atg_a_o(t, OC(GT), TYPE(BL), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(U6), cond_bl_e_u6_s_u6);
    atg_a_o(t, OC(LT), TYPE(BL), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), cond_bl_e_i6_s_i6);
    atg_a_o(t, OC(LT), TYPE(BL), AST_CLS(A), TYPE(I6), AST_CLS(S), TYPE(I6), cond_bl_a_i6_s_i6);
    atg_a_o(t, OC(LT), TYPE(BL), AST_CLS(E), TYPE(F6), AST_CLS(S), TYPE(F6), cond_bl_e_f6_s_f6);
    atg_a_o(t, OC(LT), TYPE(BL), AST_CLS(E), TYPE(U6), AST_CLS(E), TYPE(U6), cond_bl_e_e_);
    atg_a_o(t, OC(LT), TYPE(BL), AST_CLS(E), TYPE(U6), AST_CLS(O), TYPE(U6), cond_bl_e_u6_o_u6);
    atg_a_o(t, OC(LTE), TYPE(BL), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), cond_bl_e_i6_s_i6);
    atg_a_o(t, OC(LTE), TYPE(BL), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6), cond_bl_e_e_);
    atg_a_o(t, OC(AND), TYPE(BL), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A), atg_and);
    atg_a_o(t, OC(OR), TYPE(BL), AST_CLS(L), TYPE(_A), AST_CLS(L), TYPE(_A), atg_or);
}
