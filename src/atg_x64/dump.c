
#include "../atg_x64.h"

const char *atg_dump_strs[TYPE(_END)] = {
    [TYPE(U6)] = "%*s(U6 %lu)",
    [TYPE(I6)] = "%*s(I6 %ld)",
    [TYPE(F6)] = "%*s(F6 %lf)",
    [TYPE(SG)] = "%*s(SG \"%s\")",
    [TYPE(ER)] = "%*s(ER \n",
    [TYPE(TE)] = "%*s(TE \n",
    [TYPE(ST)] = "%*s(ST \n",
    [TYPE(ET)] = "%*s(ET \n",
    [TYPE(UN)] = "%*s(UN\n",
    [TYPE(VR)] = "%*s(VR \n"
};

const char *atg_dump_end = "%*s)";

const char *atg_dump_idnt = "";

const char *atg_dump_nl = "\n";

const char *atg_sym_str = "`%s\n";

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

static atg_stat atg_dump_e(atg *t, gen *g, te *an, err **e, FILE *f, size_t idnt) {
    vr *v = vr_i(5, g->va, (void*) te_f);
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(f))));
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_end))));
    dump_idnt(g, &v, idnt);
    if (gen_a(g, GEN_OP(CALLV), gen_call_v(g, v), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat dump_v(atg *t, gen *g, te *restrict an, err **e, FILE *f, size_t idnt, te *restrict type, te *restrict cd, bool nl);

static atg_stat dump_te(atg *t, gen *g, te *restrict an, err **e, FILE *f, size_t idnt, te *restrict type, te *restrict cd) {
    atg_stat stat = ATG_STAT(OK);
    vr *v = vr_i(6, g->va, (void*) te_f);
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(f))));
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(TE)]))));
    dump_idnt(g, &v, idnt);
    if (gen_a(g, GEN_OP(CALLV), gen_call_v(g, v), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    for (size_t i = 2; i < type->l; i++) if ((stat = dump_v(t, g, an, e, f, idnt + 1, type->d[i].p, gen_idx_m(g, type_g_x64_type(type->d[i].p), 2, te_c(cd), atg_te_idx_d(g, i - 2)), true)) != ATG_STAT(OK)) return stat;
    atg_dump_e(t, g, an, e, f, idnt);
    return stat;
}

static atg_stat dump_un(atg *t, gen *g, te *restrict an, err **e, FILE *f, size_t idnt, te *restrict type, te *restrict cd) {
    atg_stat stat = ATG_STAT(OK);
    uint32_t el = t->lc++, cl = 0;
    size_t eid = 0;
    te *h;
    vr *v = vr_i(6, g->va, (void*) te_f);
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(f))));
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(UN)]))));
    dump_idnt(g, &v, idnt);
    if (gen_a(g, GEN_OP(CALLV), gen_call_v(g, v), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    h = ((tbl*) type->d[2].p)->i->h;
    while (h) {
        if (h->d[2].p && gen_a(g, GEN_OP(NE), gen_idx_m(g, X64_TYPE(U6), 2, te_c(cd), atg_te_idx_d(g, 0)), gen_data(g, X64_TYPE(U6), U6(eid)), gen_lbl(g, cl = t->lc++)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        if ((stat = dump_v(t, g, an, e, f, idnt + 1, ((te*) h->d[0].p)->d[2].p, gen_idx_m(g, type_g_x64_type(((te*) h->d[0].p)->d[2].p), 2, te_c(cd), atg_te_idx_d(g, 1)), false)) != ATG_STAT(OK)) return stat;
        mc *sym = ((te*) h->d[0].p)->d[0].p;
        if (gen_a(g, GEN_OP(CALLV), gen_call_m(g, 3, gen_data(g, X64_TYPE(M), P(f)), gen_data(g, X64_TYPE(M), P(atg_sym_str)), gen_data(g, X64_TYPE(S), P(sym))), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        if (h->d[2].p) {
            if (gen_a(g, GEN_OP(JMP), gen_lbl(g, el), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
            if (gen_a(g, GEN_OP(LBL), gen_lbl(g, cl), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        }
        h = h->d[2].p;
        eid++;
    }
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, el), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    atg_dump_e(t, g, an, e, f, idnt);
    return stat;
}

static atg_stat dump_err(atg *t, gen *g, te *restrict an, err **e, FILE *f, size_t idnt, te *restrict cd) {
    vr *v = vr_i(6, g->va, (void*) te_f);
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(f))));
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(ER)]))));
    dump_idnt(g, &v, idnt);
    if (gen_a(g, GEN_OP(CALLV), gen_call_v(g, v), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(CALLV), gen_call_m(g, 2, cd, gen_data(g, X64_TYPE(U3), U3(1))), gen_data(g, X64_TYPE(M), P(err_p)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    atg_dump_e(t, g, an, e, f, idnt);
    return ATG_STAT(OK);
}

static vr *dump_vr_i(gen *g, FILE *f, type t, size_t idnt) {
    vr *v = vr_i(6, g->va, (void*) te_f);
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(f))));
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[t]))));
    dump_idnt(g, &v, idnt);
    return v;
}

static atg_stat dump_v(atg *t, gen *g, te *restrict an, err **e, FILE *f, size_t idnt, te *restrict type, te *restrict cd, bool nl) {
    uint32_t ti;
    atg_stat stat = ATG_STAT(OK);
    te *tetmp;
    vr *v = NULL;
    switch (type->d[1].u4) {
        case TYPE(U6):
            v = dump_vr_i(g, f, TYPE(U6), idnt);
            vr_ab(&v, P(cd));
            break;
        case TYPE(I6):
            v = dump_vr_i(g, f, TYPE(I6), idnt);
            vr_ab(&v, P(cd));
            break;
        case TYPE(F6):
            v = dump_vr_i(g, f, TYPE(F6), idnt);
            vr_ab(&v, P(cd));
            break;
        case TYPE(SG):
            v = dump_vr_i(g, f, TYPE(SG), idnt);
            if (gen_a(g, GEN_OP(REF), gen_tmp(g, X64_TYPE(M), ti = t->tc++), cd, gen_data(g, X64_TYPE(U3), U3(offsetof(mc, d)))) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
            vr_ab(&v, P(gen_tmp(g, X64_TYPE(M), ti)));
            break;
        case TYPE(TE):
            if (gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(M), ti = t->tc++), cd, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
            tetmp = gen_tmp(g, X64_TYPE(M), ti);
            if ((stat = dump_te(t, g, an, e, f, idnt, type, tetmp)) != ATG_STAT(OK)) return stat;
            te_f(tetmp);
            break;
        case TYPE(UN):
            if (gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(M), ti = t->tc++), cd, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
            tetmp = gen_tmp(g, X64_TYPE(M), ti);
            if ((stat = dump_un(t, g, an, e, f, idnt, type, tetmp)) != ATG_STAT(OK)) return stat;
            te_f(tetmp);
            break;
        case TYPE(ER):
            if ((stat = dump_err(t, g, an, e, f, idnt, cd)) != ATG_STAT(OK)) return stat;
            break;
        default:
            return atg_err(t, an, e, "atg inv dump type");
    }
    if (v && gen_a(g, GEN_OP(CALLV), gen_call_v(g, v), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (nl && gen_a(g, GEN_OP(CALLV), gen_call_m(g, 2, gen_data(g, X64_TYPE(M), P(stdout)), gen_data(g, X64_TYPE(M), P(atg_dump_nl))), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return stat;
}

static atg_stat dump_vd(atg *t, gen *g, te *restrict an, err **e, te *restrict cd) {
    atg_stat stat = ATG_STAT(OK);
    te *type;
    FILE *f = NULL;
    if ((stat = dump_g_fd(t, an, e, &f)) != ATG_STAT(OK)) return stat;
    if (ast_g_t(an->d[6].p, &type) != AST_STAT(OK)) return atg_err(t, an, e, "atg ast_g_t");
    return dump_v(t, g, an, e, f, 0, type, cd, true);
}

static atg_stat dump_vd_s_u5_aply(atg *t, gen *g, te *an, err **e) {
    return dump_vd(t, g, an, e, te_c(atg_g_g(an->d[6].p)->d[1].p));
}

static atg_stat dump_vd_s_u5_e_i6(atg *t, gen *g, te *an, err **e) {
    return dump_vd(t, g, an, e, var_arg(g, ((te*) an->d[6].p)->d[3].p, X64_TYPE(I6)));
}

static atg_stat dump_vd_s_u5_oa_(atg *t, gen *g, te *an, err **e) {
    return dump_vd(t, g, an, e, te_c(atg_g_g(an->d[6].p)->d[1].p));
}

static atg_stat dump_vd_s_u5_e_f6(atg *t, gen *g, te *an, err **e) {
    return dump_vd(t, g, an, e, var_arg(g, ((te*) an->d[6].p)->d[3].p, X64_TYPE(F6)));
}

static atg_stat dump_vd_s_u5_e_sg(atg *t, gen *g, te *an, err **e) {
    return dump_vd(t, g, an, e, var_arg(g, ((te*) an->d[6].p)->d[3].p, X64_TYPE(M)));
}

static atg_stat dump_vd_s_u5_z(atg *t, gen *g, te *an, err **e) {
    atg_stat stat = ATG_STAT(OK);
    te *type;
    FILE *f = NULL;
    if ((stat = dump_g_fd(t, an, e, &f)) != ATG_STAT(OK)) return stat;
    if (ast_g_t(an->d[6].p, &type) != AST_STAT(OK)) return atg_err(t, an, e, "atg ast_g_t");
    return dump_v(t, g, an, e, f, 0, type->d[2].p, te_c(atg_g_g(an->d[6].p)->d[1].p), true);
}

static atg_stat dump_vd_s_u5_e_te(atg *t, gen *g, te *an, err **e) {
    atg_stat stat = ATG_STAT(OK);
    te *type, *lte = ((te*) an->d[6].p)->d[3].p;
    uint32_t idnt = 0;
    FILE *f = NULL;
    if ((stat = dump_g_fd(t, an, e, &f)) != ATG_STAT(OK)) return stat;
    if (ast_g_t(an->d[6].p, &type) != AST_STAT(OK)) return atg_err(t, an, e, "atg ast_g_t");
    te* cd = var_arg(g, lte, X64_TYPE(M));
    if ((stat = dump_te(t, g, an, e, f, idnt, type, cd)) != ATG_STAT(OK)) return stat;
    te_f(cd);
    if (gen_a(g, GEN_OP(CALLV), gen_call_m(g, 2, gen_data(g, X64_TYPE(M), P(stdout)), gen_data(g, X64_TYPE(M), P(atg_dump_nl))), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return stat;
}

static atg_stat dump_vd_s_u5_e_st(atg *t, gen *g, te *an, err **e) {
    atg_stat stat = ATG_STAT(OK);
    te *lte = ((te*) an->d[6].p)->d[3].p, *h;
    uint32_t idnt = 0, eid = 0;
    FILE *f = NULL;
    if ((stat = dump_g_fd(t, an, e, &f)) != ATG_STAT(OK)) return stat;
    vr *v = vr_i(6, g->va, (void*) te_f);
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(f))));
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(ST)]))));
    dump_idnt(g, &v, idnt);
    if (gen_a(g, GEN_OP(CALLV), gen_call_v(g, v), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    h = ((tbl*) ((te*) lte->d[2].p)->d[2].p)->i->h;
    while (h) {
        if ((stat = dump_v(t, g, an, e, f, idnt + 1, ((te*) h->d[0].p)->d[2].p, gen_idx_m(g, type_g_x64_type(((te*) h->d[0].p)->d[2].p), 2, var_arg(g, lte, X64_TYPE(M)), atg_te_idx_d(g, eid)), false)) != ATG_STAT(OK)) return stat;
        mc *sym = ((te*) h->d[0].p)->d[0].p;
        if (gen_a(g, GEN_OP(CALLV), gen_call_m(g, 3, gen_data(g, X64_TYPE(M), P(f)), gen_data(g, X64_TYPE(M), P(atg_sym_str)), gen_data(g, X64_TYPE(S), P(sym))), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        h = h->d[2].p;
        eid++;
    }
    atg_dump_e(t, g, an, e, f, idnt);
    if (gen_a(g, GEN_OP(CALLV), gen_call_m(g, 2, gen_data(g, X64_TYPE(M), P(stdout)), gen_data(g, X64_TYPE(M), P(atg_dump_nl))), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return stat;
}

static atg_stat dump_vd_s_u5_e_et(atg *t, gen *g, te *an, err **e) {
    atg_stat stat = ATG_STAT(OK);
    te *lte = ((te*) an->d[6].p)->d[3].p, *h;
    uint32_t idnt = 0;
    FILE *f = NULL;
    if ((stat = dump_g_fd(t, an, e, &f)) != ATG_STAT(OK)) return stat;
    vr *v = vr_i(6, g->va, (void*) te_f);
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(f))));
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(ET)]))));
    dump_idnt(g, &v, idnt);
    if (gen_a(g, GEN_OP(CALLV), gen_call_v(g, v), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    h = ((tbl*) ((te*) lte->d[2].p)->d[2].p)->i->h;
    while (h) {
        if ((stat = dump_v(t, g, an, e, f, idnt + 1, ((te*) h->d[0].p)->d[2].p, gen_data(g, type_g_x64_type(((te*) h->d[0].p)->d[2].p), ((te*) h->d[0].p)->d[1]), false)) != ATG_STAT(OK)) return stat;
        mc *sym = ((te*) h->d[0].p)->d[0].p;
        if (gen_a(g, GEN_OP(CALLV), gen_call_m(g, 3, gen_data(g, X64_TYPE(M), P(f)), gen_data(g, X64_TYPE(M), P(atg_sym_str)), gen_data(g, X64_TYPE(S), P(sym))), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        h = h->d[2].p;
    }
    atg_dump_e(t, g, an, e, f, idnt);
    if (gen_a(g, GEN_OP(CALLV), gen_call_m(g, 2, gen_data(g, X64_TYPE(M), P(stdout)), gen_data(g, X64_TYPE(M), P(atg_dump_nl))), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return stat;
}

static atg_stat dump_vd_s_u5_e_vr(atg *t, gen *g, te *an, err **e) {
    atg_stat stat = ATG_STAT(OK);
    te *type, *lte = ((te*) an->d[6].p)->d[3].p;
    uint32_t idnt = 0, tl = t->lc++, el = t->lc++, tv = t->tc++;
    FILE *f = NULL;
    if ((stat = dump_g_fd(t, an, e, &f)) != ATG_STAT(OK)) return stat;
    if (ast_g_t(an->d[6].p, &type) != AST_STAT(OK)) return atg_err(t, an, e, "atg ast_g_t");
    type = type->d[2].p;
    vr *v = vr_i(6, g->va, (void*) te_f);
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(f))));
    vr_ab(&v, P(gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(VR)]))));
    dump_idnt(g, &v, idnt);
    if (gen_a(g, GEN_OP(CALLV), gen_call_v(g, v), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(EQ), gen_idx_m(g, X64_TYPE(U6), 2, gen_stkv(g, X64_TYPE(M), ast_lst_tbl_e_g_i(lte)), gen_data(g, X64_TYPE(U3), U3(offsetof(vr, l)))), gen_data(g, X64_TYPE(U6), U6(0)), gen_lbl(g, el)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(U6), tv), gen_data(g, X64_TYPE(U6), U6(0)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, tl), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if ((stat = dump_v(t, g, an, e, f, idnt + 1, type, gen_idx_m(g, type_g_x64_type(type), 3, gen_stkv(g, X64_TYPE(M), ast_lst_tbl_e_g_i(lte)), gen_data(g, X64_TYPE(U3), U3(sizeof(void*) * 5)), gen_tmp(g, X64_TYPE(U6), tv)), true)) != ATG_STAT(OK)) return stat;
    if (gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(U6), tv), gen_tmp(g, X64_TYPE(U6), tv), gen_data(g, X64_TYPE(U6), U6(1))) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(LT), gen_tmp(g, X64_TYPE(U6), tv), gen_idx_m(g, X64_TYPE(U6), 2, gen_stkv(g, X64_TYPE(M), ast_lst_tbl_e_g_i(lte)), gen_data(g, X64_TYPE(U3), U3(offsetof(vr, l)))), gen_lbl(g, tl)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, el), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    atg_dump_e(t, g, an, e, f, idnt);
    if (gen_a(g, GEN_OP(CALLV), gen_call_m(g, 2, gen_data(g, X64_TYPE(M), P(stdout)), gen_data(g, X64_TYPE(M), P(atg_dump_nl))), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return stat;
}

static atg_stat dump_vd_s_u5_e_un(atg *t, gen *g, te *an, err **e) {
    atg_stat stat = ATG_STAT(OK);
    te *type, *lte = ((te*) an->d[6].p)->d[3].p;
    FILE *f = NULL;
    if ((stat = dump_g_fd(t, an, e, &f)) != ATG_STAT(OK)) return stat;
    if (ast_g_t(an->d[6].p, &type) != AST_STAT(OK)) return atg_err(t, an, e, "atg ast_g_t");
    te *cd = var_arg(g, lte, X64_TYPE(M));
    if ((stat = dump_un(t, g, an, e, f, 0, type, cd)) != ATG_STAT(OK)) return stat;
    te_f(cd);
    if (gen_a(g, GEN_OP(CALLV), gen_call_m(g, 2, gen_data(g, X64_TYPE(M), P(stdout)), gen_data(g, X64_TYPE(M), P(atg_dump_nl))), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return stat;
}

static atg_stat dump_vd_s_u5_e_er(atg *t, gen *g, te *an, err **e) {
    atg_stat stat = ATG_STAT(OK);
    te *lte = ((te*) an->d[6].p)->d[3].p;
    FILE *f = NULL;
    if ((stat = dump_g_fd(t, an, e, &f)) != ATG_STAT(OK)) return stat;
    if ((stat = dump_err(t, g, an, e, f, 0, var_arg(g, lte, X64_TYPE(M)))) != ATG_STAT(OK)) return stat;
    if (gen_a(g, GEN_OP(CALLV), gen_call_m(g, 2, gen_data(g, X64_TYPE(M), P(stdout)), gen_data(g, X64_TYPE(M), P(atg_dump_nl))), gen_data(g, X64_TYPE(M), P(fprintf)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return stat;
}

void atg_dump(atg *t) {
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(I6), dump_vd_s_u5_aply);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(U6), dump_vd_s_u5_aply);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(F6), dump_vd_s_u5_aply);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(I6), dump_vd_s_u5_e_i6);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(O), TYPE(I6), dump_vd_s_u5_oa_);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(O), TYPE(SG), dump_vd_s_u5_oa_);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(A), TYPE(SG), dump_vd_s_u5_oa_);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(F6), dump_vd_s_u5_e_f6);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(SG), dump_vd_s_u5_e_sg);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(TE), dump_vd_s_u5_e_te);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(ST), dump_vd_s_u5_e_st);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(ET), dump_vd_s_u5_e_et);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(VR), dump_vd_s_u5_e_vr);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(UN), dump_vd_s_u5_e_un);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(O), TYPE(UN), dump_vd_s_u5_oa_);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(E), TYPE(ER), dump_vd_s_u5_e_er);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(Z), TYPE(F6), dump_vd_s_u5_z);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(Z), TYPE(I6), dump_vd_s_u5_z);
    atg_a_o(t, OC(DUMP), TYPE(VD), AST_CLS(S), TYPE(U5), AST_CLS(Z), TYPE(SG), dump_vd_s_u5_z);
}
