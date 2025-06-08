
#include "../atg_x64.h"

const char *atg_dump_strs[TYPE(_END)] = {
    [TYPE(U6)] = "%*s(U6 %lu)\n",
    [TYPE(I6)] = "%*s(I6 %ld)\n",
    [TYPE(F6)] = "%*s(F6 %lf)\n",
    [TYPE(SG)] = "%*s(SG \"%s\")\n",
    [TYPE(TE)] = "%*s(TE \n",
    [TYPE(ST)] = "%*s(ST \n",
    [TYPE(ET)] = "%*s(ET \n",
    [TYPE(VR)] = "%*s(VR \n"
};

const char *atg_dump_end = "%*s)\n";

const char *atg_dump_idnt = "";

const char *atg_sym_str = "%*s`%s ";

static atg_stat dump_g_fd(atg *t, te *an, err **e, FILE **f) {
    uint32_t fd = ((te*) an->d[5].p)->d[4].u5;
    if (fd == 0 || fd > 2) return atg_err(t, an, e, "atg inv dump fd");
    *f = fd == 1 ? stdout : stderr;
    return ATG_STAT(OK);
}

static void dump_idnt(gen *g, vr **v, uint32_t n) {
    vr_ab(v, P(gen_data(g, X64_TYPE(U5), U5(n))));
    vr_ab(v, P(gen_data(g, X64_TYPE(M), P(atg_dump_idnt))));
}

static atg_stat atg_dump_e(atg *t, gen *g, te *an, err **e, FILE *f, size_t idnt, gen_op go) {
    vr *v = vr_i(5, g->va, (void*) te_f);
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(f))));
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_end))));
    dump_idnt(g, &v, idnt);
    if (gen_a(g, go, gen_call_v(g, v), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat dump_v(atg *t, gen *g, te *restrict an, err **e, gen_op go, FILE *f, size_t idnt, te *restrict type, te *restrict cd, mc *sym) {
    uint32_t ti;
    atg_stat stat = ATG_STAT(OK);
    if (sym && gen_a(g, go, gen_call_m(g, 5, gen_data(g, X64_TYPE(M), P(f)), gen_data(g, X64_TYPE(M), P(atg_sym_str)), gen_data(g, X64_TYPE(U5), U5(idnt)), gen_data(g, X64_TYPE(M), P(atg_dump_idnt)), gen_data(g, X64_TYPE(S), P(sym))), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    vr *v = vr_i(6, g->va, (void*) te_f);
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(f))));
    switch (type->d[1].u4) {
        case TYPE(U6):
            vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(U6)]))));
            dump_idnt(g, &v, sym ? 0 :idnt);
            vr_ab(&v, P(cd));
            break;
        case TYPE(I6):
            vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)]))));
            dump_idnt(g, &v, sym ? 0 :idnt);
            vr_ab(&v, P(cd));
            break;
        case TYPE(F6):
            vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(F6)]))));
            dump_idnt(g, &v, sym ? 0 :idnt);
            vr_ab(&v, P(cd));
            break;
        case TYPE(SG):
            vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(SG)]))));
            dump_idnt(g, &v, sym ? 0 :idnt);
            if (gen_a(g, GEN_OP(REF), gen_tmp(g, X64_TYPE(M), ti = t->tc++), cd, gen_data(g, X64_TYPE(U3), U3(offsetof(mc, d)))) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
            vr_ab(&v, P(gen_tmp(g, X64_TYPE(M), ti)));
            break;
        default:
            vr_f(v);
            return atg_err(t, an, e, "atg inv dump type");
    }
    if (gen_a(g, go, gen_call_v(g, v), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return stat;
}

static atg_stat dump_vd(atg *t, gen *g, te *restrict an, err **e, te *restrict cd) {
    atg_stat stat = ATG_STAT(OK);
    te *type;
    FILE *f;
    if ((stat = dump_g_fd(t, an, e, &f)) != ATG_STAT(OK)) return stat;
    if (ast_g_t(an->d[6].p, &type) != AST_STAT(OK)) return atg_err(t, an, e, "atg ast_g_t");
    gen_op go = GEN_OP(CALLV);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    return dump_v(t, g, an, e, go, f, 0, type, cd, NULL);
}

static atg_stat dump_vd_s_u5_aply(atg *t, gen *g, te *an, err **e) {
    return dump_vd(t, g, an, e, te_c(atg_g_g(an->d[6].p)->d[1].p));
}

static atg_stat dump_vd_s_u5_e_i6(atg *t, gen *g, te *an, err **e) {
    return dump_vd(t, g, an, e, var_arg(g, ((te*) an->d[6].p)->d[3].p, X64_TYPE(I6)));
}

static atg_stat dump_vd_s_u5_e_f6(atg *t, gen *g, te *an, err **e) {
    return dump_vd(t, g, an, e, var_arg(g, ((te*) an->d[6].p)->d[3].p, X64_TYPE(F6)));
}

static atg_stat dump_vd_s_u5_e_te(atg *t, gen *g, te *an, err **e) {
    atg_stat stat = ATG_STAT(OK);
    te *type, *lte = ((te*) an->d[6].p)->d[3].p;
    uint32_t idnt = 0;
    FILE *f;
    if ((stat = dump_g_fd(t, an, e, &f)) != ATG_STAT(OK)) return stat;
    if (ast_g_t(an->d[6].p, &type) != AST_STAT(OK)) return atg_err(t, an, e, "atg ast_g_t");
    vr *v = vr_i(6, g->va, (void*) te_f);
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(f))));
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(TE)]))));
    dump_idnt(g, &v, idnt);
    gen_op go = GEN_OP(CALLV);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    if (gen_a(g, go, gen_call_v(g, v), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    for (size_t i = 2; i < type->l; i++) if ((stat = dump_v(t, g, an, e, go, f, idnt + 1, type->d[i].p, gen_idx_m(g, type_g_x64_type(type->d[i].p), 2, gen_stkv(g, X64_TYPE(M), ast_lst_tbl_e_g_i(lte)), atg_te_idx_d(g, i - 2)), NULL)) != ATG_STAT(OK)) return stat;
    go = GEN_OP(CALLV);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    atg_dump_e(t, g, an, e, f, idnt, go);
    return stat;
}

static atg_stat dump_vd_s_u5_e_st(atg *t, gen *g, te *an, err **e) {
    atg_stat stat = ATG_STAT(OK);
    te *lte = ((te*) an->d[6].p)->d[3].p, *h;
    uint32_t idnt = 0, eid = 0;
    FILE *f;
    if ((stat = dump_g_fd(t, an, e, &f)) != ATG_STAT(OK)) return stat;
    vr *v = vr_i(6, g->va, (void*) te_f);
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(f))));
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(ST)]))));
    dump_idnt(g, &v, idnt);
    gen_op go = GEN_OP(CALLV);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    if (gen_a(g, go, gen_call_v(g, v), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    h = ((tbl*) ((te*) lte->d[2].p)->d[2].p)->i->h;
    while (h) {
        if ((stat = dump_v(t, g, an, e, go, f, idnt + 1, ((te*) h->d[0].p)->d[2].p, gen_idx_m(g, type_g_x64_type(((te*) h->d[0].p)->d[2].p), 2, gen_stkv(g, X64_TYPE(M), ast_lst_tbl_e_g_i(lte)), atg_te_idx_d(g, eid)), ((te*) h->d[0].p)->d[0].p)) != ATG_STAT(OK)) return stat;
        h = h->d[2].p;
        eid++;
    }
    go = GEN_OP(CALLV);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    atg_dump_e(t, g, an, e, f, idnt, go);
    return stat;
}

static atg_stat dump_vd_s_u5_e_et(atg *t, gen *g, te *an, err **e) {
    atg_stat stat = ATG_STAT(OK);
    te *lte = ((te*) an->d[6].p)->d[3].p, *h;
    uint32_t idnt = 0;
    FILE *f;
    if ((stat = dump_g_fd(t, an, e, &f)) != ATG_STAT(OK)) return stat;
    vr *v = vr_i(6, g->va, (void*) te_f);
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(f))));
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(ET)]))));
    dump_idnt(g, &v, idnt);
    gen_op go = GEN_OP(CALLV);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    if (gen_a(g, go, gen_call_v(g, v), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    h = ((tbl*) ((te*) lte->d[2].p)->d[2].p)->i->h;
    while (h) {
        if ((stat = dump_v(t, g, an, e, go, f, idnt + 1, ((te*) h->d[0].p)->d[2].p, gen_data(g, type_g_x64_type(((te*) h->d[0].p)->d[2].p), ((te*) h->d[0].p)->d[1]), ((te*) h->d[0].p)->d[0].p)) != ATG_STAT(OK)) return stat;
        h = h->d[2].p;
    }
    go = GEN_OP(CALLV);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    atg_dump_e(t, g, an, e, f, idnt, go);
    return stat;
}

static atg_stat dump_vd_s_u5_e_vr(atg *t, gen *g, te *an, err **e) {
    atg_stat stat = ATG_STAT(OK);
    te *type, *lte = ((te*) an->d[6].p)->d[3].p;
    uint32_t idnt = 0, tl = t->lc++, tv = t->tc++;
    FILE *f;
    if ((stat = dump_g_fd(t, an, e, &f)) != ATG_STAT(OK)) return stat;
    if (ast_g_t(an->d[6].p, &type) != AST_STAT(OK)) return atg_err(t, an, e, "atg ast_g_t");
    type = type->d[2].p;
    vr *v = vr_i(6, g->va, (void*) te_f);
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(f))));
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(VR)]))));
    dump_idnt(g, &v, idnt);
    gen_op go = GEN_OP(CALLV);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    if (gen_a(g, go, gen_call_v(g, v), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(U6), tv), gen_data(g, X64_TYPE(U6), U6(0)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, tl), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if ((stat = dump_v(t, g, an, e, GEN_OP(CALLV), f, idnt + 1, type, gen_idx_m(g, type_g_x64_type(type), 3, gen_stkv(g, X64_TYPE(M), ast_lst_tbl_e_g_i(lte)), gen_data(g, X64_TYPE(U3), U3(sizeof(void*) * 5)), gen_tmp(g, X64_TYPE(U6), tv)), NULL)) != ATG_STAT(OK)) return stat;
    if (gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(U6), tv), gen_tmp(g, X64_TYPE(U6), tv), gen_data(g, X64_TYPE(U6), U6(1))) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(LT), gen_tmp(g, X64_TYPE(U6), tv), gen_idx_m(g, X64_TYPE(U6), 2, gen_stkv(g, X64_TYPE(M), ast_lst_tbl_e_g_i(lte)), gen_data(g, X64_TYPE(U3), U3(sizeof(void*) * 2))), gen_lbl(g, tl)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    go = GEN_OP(CALLV);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    atg_dump_e(t, g, an, e, f, idnt, go);
    return stat;
}

void atg_dump(atg *t) {
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(I6), dump_vd_s_u5_aply);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(U6), dump_vd_s_u5_aply);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(F6), dump_vd_s_u5_aply);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(I6), dump_vd_s_u5_e_i6);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(F6), dump_vd_s_u5_e_f6);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(TE), dump_vd_s_u5_e_te);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(ST), dump_vd_s_u5_e_st);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(ET), dump_vd_s_u5_e_et);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(VR), dump_vd_s_u5_e_vr);
}
