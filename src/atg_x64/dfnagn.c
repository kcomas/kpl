
#include "../atg_x64.h"

#define DFNES(t, T) static atg_stat dfn_##t##_e_##t##_s_##t(atg *t, gen *g, te *an, err **e) { \
    uint32_t vid = ast_lst_tbl_e_g_i(((te*) an->d[5].p)->d[3].p); \
    if (gen_a(g, GEN_OP(SET), gen_stkv(g, X64_TYPE(T), vid), gen_data(g, X64_TYPE(T), ((te*) an->d[6].p)->d[4]), NULL)) return atg_err(t, an, e, __FUNCTION__); \
    return ATG_STAT(OK); \
}

DFNES(i6, I6);
DFNES(u6, U6);
DFNES(f6, F6)

static atg_stat dfn_i6_e_i6_e_i6(atg *t, gen *g, te *an, err **e) {
    te *l = an->d[5].p, *r = an->d[6].p;
    if (gen_a(g, GEN_OP(SET), var_arg(g, l->d[3].p, X64_TYPE(I6)), var_arg(g, r->d[3].p, X64_TYPE(I6)), NULL)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat dfn_t_e_t_oa_t(atg *t, gen *g, te *an, err **e) {
    te *l = an->d[5].p, *r = atg_g_g(an->d[6].p)->d[1].p;
    if (gen_a(g, GEN_OP(SET), var_arg(g, l->d[3].p, type_g_x64_type(((te*) l->d[3].p)->d[2].p)), te_c(r), NULL)) return atg_err(t, an, e, __FUNCTION__);
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
    te *lte = ((te*) an->d[5].p)->d[3].p;
    te *rgc = atg_g_g(an->d[6].p)->d[1].p;
    if (gen_a(g, GEN_OP(SET), gen_stkv(g, type_g_x64_type(lte->d[2].p), ast_lst_tbl_e_g_i(lte)), te_c(rgc), NULL)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
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
    atg_a_o(t, OC(DFN), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6), dfn_i6_e_i6_e_i6);
    atg_a_o(t, OC(DFN), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(O), TYPE(I6), dfn_t_e_t_oa_t);
    atg_a_o(t, OC(DFN), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(A), TYPE(I6), dfn_t_e_t_oa_t);
    atg_a_o(t, OC(DFN), TYPE(U6), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(U6), dfn_u6_e_u6_s_u6);
    atg_a_o(t, OC(DFN), TYPE(F6), AST_CLS(E), TYPE(F6), AST_CLS(S), TYPE(F6), dfn_f6_e_f6_s_f6);
    atg_a_o(t, OC(DFN), TYPE(FN), AST_CLS(E), TYPE(FN), AST_CLS(S), TYPE(_G), dfn_fn_e_fn_s__g);
    atg_a_o(t, OC(DFN), TYPE(NF), AST_CLS(E), TYPE(NF), AST_CLS(S), TYPE(_G), dfn_fn_e_fn_s__g);
    atg_a_o(t, OC(DFN), TYPE(SG), AST_CLS(E), TYPE(SG), AST_CLS(A), TYPE(SG), dfn_t_e_t_oa_t);
    atg_a_o(t, OC(DFN), TYPE(TE), AST_CLS(E), TYPE(TE), AST_CLS(V), TYPE(TE), dfn_tevrst_e_tevrst_vo_tevrst);
    atg_a_o(t, OC(DFN), TYPE(VR), AST_CLS(E), TYPE(VR), AST_CLS(V), TYPE(VR), dfn_tevrst_e_tevrst_vo_tevrst);
    atg_a_o(t, OC(DFN), TYPE(ST), AST_CLS(E), TYPE(ST), AST_CLS(O), TYPE(ST), dfn_tevrst_e_tevrst_vo_tevrst);
    atg_a_o(t, OC(AGN), TYPE(I6), AST_CLS(A), TYPE(I6), AST_CLS(O), TYPE(I6), agn_i6_a_i6_o_i6);
    atg_a_o(t, OC(AGN), TYPE(I6), AST_CLS(Z), TYPE(I6), AST_CLS(O), TYPE(I6), agn_i6_z_i6_o_i6);
}
