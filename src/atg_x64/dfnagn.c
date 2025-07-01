
#include "../atg_x64.h"

#define DFNES(t, T) static atg_stat dfn_##t##_e_##t##_s_##t(atg *t, gen *g, te *an, err **e) { \
    uint32_t vid = ast_lst_tbl_e_g_i(((te*) an->d[5].p)->d[3].p); \
    if (gen_a(g, GEN_OP(SET), gen_stkv(g, X64_TYPE(T), vid), gen_data(g, X64_TYPE(T), ((te*) an->d[6].p)->d[4]), NULL)) return atg_err(t, an, e, __FUNCTION__); \
    return ATG_STAT(OK); \
}

DFNES(i6, I6);
DFNES(u6, U6);
DFNES(f6, F6)

static atg_stat da_in_loop(atg *t, gen *g, te *restrict an, err **e, te *restrict lte, const char *pf) {
    te *tn = lte->d[2].p;
    if (type_is_ref(tn->d[1].u4) && inloop(an)) {
        void *fn = type_ref_g_des(tn->d[1].u4);
        if (!fn) return atg_err(t, an, e, "atg inv des for define in loop");
        if (gen_a(g, GEN_OP(CALL), gen_call_m(g, 1, var_arg(g, lte, type_g_x64_type(tn))), gen_data(g, X64_TYPE(M), P(fn)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, pf);
    }
    return ATG_STAT(OK);
}

static atg_stat dfnagn_e_e_(atg *t, gen *g, te *an, err **e) {
    te *l = ((te*) an->d[5].p)->d[3].p, *r = ((te*) an->d[6].p)->d[3].p;
    if (gen_a(g, GEN_OP(SET), var_arg(g, l, type_g_x64_type(l->d[2].p)), var_arg(g, r, type_g_x64_type(r->d[2].p)), NULL)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat dfn_t_e_t_oa_t(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    te *l = an->d[5].p, *r = atg_g_g(an->d[6].p)->d[1].p;
    te *lte = l->d[3].p;
    if ((stat = da_in_loop(t, g, an, e, lte, __FUNCTION__)) != ATG_STAT(OK)) return stat;
    if (gen_a(g, GEN_OP(SET), var_arg(g, lte, type_g_x64_type(lte->d[2].p)), te_c(r), NULL)) return atg_err(t, an, e, __FUNCTION__);
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

static atg_stat dfn_tevrst_e_tevrst_vo_tevrst(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    te *lte = ((te*) an->d[5].p)->d[3].p;
    te *rgc = atg_g_g(an->d[6].p)->d[1].p;
    if ((stat = da_in_loop(t, g, an, e, lte, __FUNCTION__)) != ATG_STAT(OK)) return stat;
    if (gen_a(g, GEN_OP(SET), gen_stkv(g, type_g_x64_type(lte->d[2].p), ast_lst_tbl_e_g_i(lte)), te_c(rgc), NULL)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat dfn_un_e_un_z(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    uint32_t ti = t->tc++;
    size_t eid;
    te *kv;
    te *lte = ((te*) an->d[5].p)->d[3].p, *zn = an->d[6].p, *unt = an->d[3].p;
    if ((stat = atg_te_init(t, g, an, e, unt, 2, ti)) != ATG_STAT(OK)) return stat;
    if (tbl_g_i(unt->d[2].p, zn->d[5], &kv) != TBL_STAT(OK)) return atg_err(t, an, e, "atg inv key for un tbl");
    if (lst_g_i(((tbl*) unt->d[2].p)->i, P(kv), &eid) != LST_STAT(OK)) return atg_err(t, an, e, "atg inv idx for un tbl itm");
    if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ti), gen_data(g, X64_TYPE(U3), U3(offsetof(te, d)))), gen_data(g, X64_TYPE(U6), U6(eid)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if ((stat = v_set_fn(t, g, an, e, zn->d[4].p, ti, 1, atg_te_idx_d)) != ATG_STAT(OK)) return stat;
    if ((stat = da_in_loop(t, g, an, e, lte, __FUNCTION__)) != ATG_STAT(OK)) return stat;
    if (gen_a(g, GEN_OP(SET), gen_stkv(g, X64_TYPE(M), ast_lst_tbl_e_g_i(lte)), gen_tmp(g, X64_TYPE(M), ti), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat agn_un_e_un_z(atg *t, gen *g, te *an, err **e) {
    te *lte = ((te*) an->d[5].p)->d[3].p;
    if (gen_a(g, GEN_OP(CALL), gen_call_m(g, 1, gen_stkv(g, X64_TYPE(M), ast_lst_tbl_e_g_i(lte))), gen_data(g, X64_TYPE(M), P(te_f)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return dfn_un_e_un_z(t, g, an, e);
}


static atg_stat agn_i6_a_i6_o_i6(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    size_t n;
    te *rc = atg_g_g(an->d[6].p)->d[1].p, *lte;
    if (ast_g_t(((te*) an->d[5].p)->d[4].p, &lte) != AST_STAT(OK)) return atg_err(t, an, e, "atg agn g tn inv");
    switch (lte->d[1].u4) {
        case TYPE(TE):
            if ((stat = aply_te_g_idx(t, an->d[5].p, e, &n, &lte)) != ATG_STAT(OK)) return stat;
            if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, var_arg(g, lte, X64_TYPE(M)), atg_te_idx_d(g, n)), te_c(rc), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
            return ATG_STAT(OK);
        // TODO check for other types
        default:
            break;
    }
    return atg_err(t, an, e, "atg inv aply agn");
}

static atg_stat agn_i6_z_i6_o_i6(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    size_t eid;
    te *r = atg_g_g(an->d[6].p)->d[1].p, *lte;
    if ((stat = z_lte_id(t, an, e, an->d[5].p, &lte, &eid)) != ATG_STAT(OK)) return stat;
    if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, var_arg(g, lte, X64_TYPE(M)), atg_te_idx_d(g, eid)), te_c(r), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

void atg_dfn(atg *t) {
    atg_a_o(t, OC(DFN), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), dfn_i6_e_i6_s_i6);
    atg_a_o(t, OC(DFN), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6), dfnagn_e_e_);
    atg_a_o(t, OC(DFN), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(O), TYPE(I6), dfn_t_e_t_oa_t);
    atg_a_o(t, OC(DFN), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(A), TYPE(I6), dfn_t_e_t_oa_t);
    atg_a_o(t, OC(DFN), TYPE(U6), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(U6), dfn_u6_e_u6_s_u6);
    atg_a_o(t, OC(DFN), TYPE(F6), AST_CLS(E), TYPE(F6), AST_CLS(S), TYPE(F6), dfn_f6_e_f6_s_f6);
    atg_a_o(t, OC(DFN), TYPE(FN), AST_CLS(E), TYPE(FN), AST_CLS(S), TYPE(_G), dfn_fn_e_fn_s__g);
    atg_a_o(t, OC(DFN), TYPE(NF), AST_CLS(E), TYPE(NF), AST_CLS(S), TYPE(_G), dfn_fn_e_fn_s__g);
    atg_a_o(t, OC(DFN), TYPE(F6), AST_CLS(E), TYPE(F6), AST_CLS(O), TYPE(F6), dfn_t_e_t_oa_t);
    atg_a_o(t, OC(DFN), TYPE(SG), AST_CLS(E), TYPE(SG), AST_CLS(A), TYPE(SG), dfn_t_e_t_oa_t);
    atg_a_o(t, OC(DFN), TYPE(SG), AST_CLS(E), TYPE(SG), AST_CLS(O), TYPE(SG), dfn_t_e_t_oa_t);
    atg_a_o(t, OC(DFN), TYPE(UN), AST_CLS(E), TYPE(UN), AST_CLS(A), TYPE(UN), dfn_t_e_t_oa_t);
    atg_a_o(t, OC(DFN), TYPE(TE), AST_CLS(E), TYPE(TE), AST_CLS(V), TYPE(TE), dfn_tevrst_e_tevrst_vo_tevrst);
    atg_a_o(t, OC(DFN), TYPE(VR), AST_CLS(E), TYPE(VR), AST_CLS(V), TYPE(VR), dfn_tevrst_e_tevrst_vo_tevrst);
    atg_a_o(t, OC(DFN), TYPE(ST), AST_CLS(E), TYPE(ST), AST_CLS(O), TYPE(ST), dfn_tevrst_e_tevrst_vo_tevrst);
    atg_a_o(t, OC(DFN), TYPE(FN), AST_CLS(E), TYPE(FN), AST_CLS(T), TYPE(FN), atg_nop);
    atg_a_o(t, OC(DFN), TYPE(UN), AST_CLS(E), TYPE(UN), AST_CLS(Z), TYPE(VD), dfn_un_e_un_z);
    atg_a_o(t, OC(AGN), TYPE(UN), AST_CLS(E), TYPE(UN), AST_CLS(Z), TYPE(VD), agn_un_e_un_z);
    atg_a_o(t, OC(AGN), TYPE(I6), AST_CLS(A), TYPE(I6), AST_CLS(O), TYPE(I6), agn_i6_a_i6_o_i6);
    atg_a_o(t, OC(AGN), TYPE(I6), AST_CLS(Z), TYPE(I6), AST_CLS(O), TYPE(I6), agn_i6_z_i6_o_i6);
    atg_a_o(t, OC(AGN), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6), dfnagn_e_e_);
    atg_a_o(t, OC(AGN), TYPE(F6), AST_CLS(E), TYPE(F6), AST_CLS(E), TYPE(F6), dfnagn_e_e_);
}
