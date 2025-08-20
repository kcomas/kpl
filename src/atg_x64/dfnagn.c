
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
    if (!type_is_ref(tn->d[1].u4) || !inloop(an)) return ATG_STAT(OK);
    void *fn = type_ref_g_des(tn);
    if (!fn) return atg_err(t, an, e, "atg inv des for define in loop");
    if (gen_a(g, GEN_OP(CALL), gen_call_m(g, 1, var_arg(g, lte, type_g_x64_type(tn))), gen_data(g, X64_TYPE(M), P(fn)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, pf);
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

static atg_stat dfn_un_e_un_e_un(atg *t, gen *g, te *an, err **e) {
    te *l = an->d[5].p, *r = an->d[6].p, *i;
    if (atg_an_var(g, &l) != ATG_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(CALL), gen_call_m(g, 1, l), gen_data(g, X64_TYPE(M), P(te_f)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (atg_an_var(g, &r) != ATG_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    i = gen_idx_m(g, X64_TYPE(I6), 2, r, gen_data(g, X64_TYPE(U3), U3(0)));
    if (gen_a(g, GEN_OP(ADD), i, te_c(i), gen_data(g, X64_TYPE(I6), I6(1))) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(SET), te_c(l), te_c(r), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
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

static atg_stat agn_st_z_st_o_st(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    size_t uid, fi = t->tc++, ai = t->tc++;
    te *lt = an->d[5].p, *lci = atg_g_g(an->d[5].p)->d[1].p, *kv;
    mc *s = lt->d[5].p;
    lt = ((te*) lt->d[4].p)->d[3].p;
    lt = lt->d[2].p;
    if (tbl_g_i(lt->d[2].p, P(s), &kv) != TBL_STAT(OK)) return atg_err(t, an, e, "atg inv key for st tbl");
    if (lst_g_i(((tbl*) lt->d[2].p)->i, P(kv), &uid) != LST_STAT(OK)) return atg_err(t, an, e, "atg inv idx for st tbl itm");
    if ((stat = atg_te_init(t, g, an, e, lt, 2, ai)) != ATG_STAT(OK)) return stat;
    if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ai), atg_te_idx_d(g, 0)), gen_data(g, X64_TYPE(U6), U6(uid)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ai), atg_te_idx_d(g, 1)), te_c(atg_g_g(an->d[6].p)->d[1].p), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(REF), gen_tmp(g, X64_TYPE(M), fi), te_c(lci), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(CALL), gen_call_m(g, 1, gen_tmp(g, X64_TYPE(M), fi)), gen_data(g, X64_TYPE(M), P(te_f)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(SET), te_c(lci), gen_tmp(g, X64_TYPE(M), ai), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat agn_vr_idx(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    uint32_t erlb = t->lc++, glb = t->lc++, ei = t->tc++, esi = t->tc++, si = t->tc++, ii = t->tc++, ui = t->tc++, ri = t->tc++;
    uint64_t esym, vsym;
    te *l = an->d[5].p, *v = l->d[4].p, *i = ((lst*) l->d[5].p)->h->d[0].p, *tkn = ((te*) an->d[1].p)->d[2].p, *r = an->d[6].p, *rt;
    if ((stat = atg_an_var(g, &i)) != ATG_STAT(OK)) return stat;
    if (v->d[2].u4 != AST_CLS(E)) return atg_err(t, an, e, "atg inv vr agn");
    v = var_arg(g, v->d[3].p, X64_TYPE(M));
    if ((stat = atg_aply_acc(t, g, an, e, te_c(v), &i, erlb)) != ATG_STAT(OK)) return stat;
    if (aply_e_vr_err_u(g, i, te_c(v), erlb, glb, ei, esi, si, ii, node_root_fname(an->d[1].p), tkn_m_g_l(tkn), tkn_m_g_c(tkn)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (atg_g_un_ev(l->d[3].p, &esym, &vsym) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv `e`v for un");
    if (chk_g_pn_fnnf_type(an)) {
        if ((stat = atg_te_init(t, g, an, e, l->d[3].p, 2, ui)) != ATG_STAT(OK)) return stat;
        if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ui), gen_data(g, X64_TYPE(U3), U3(offsetof(te, d)))), gen_data(g, X64_TYPE(U6), U6(esym)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ui), gen_data(g, X64_TYPE(U3), U3(offsetof(te, d) + sizeof(void*)))), gen_tmp(g, X64_TYPE(M), ei), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        if (gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(M), ui), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    } else if (gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(M), ei), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, glb), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if ((stat = atg_an_var(g, &r)) != ATG_STAT(OK)) return stat;
    if (ast_g_t(an->d[6].p, &rt) != AST_STAT(OK)) return atg_err(t, an, e, "atg agn vr inv rt");
    x64_type xt = gen_var_g_t(r);
    if (gen_a(g, GEN_OP(SET), gen_tmp(g, xt, ri), r, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (type_is_ref(rt->d[1].u4) && inc_ref_u(g, xt, ri) != GEN_STAT(OK)) return atg_err(t, an, e, "atg agn vr rt ref inf");
    if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 3, v, gen_data(g, X64_TYPE(U3), U3(offsetof(vr, d))), te_c(i)), gen_tmp(g, xt, ri), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(NOP), gen_tmp(g, xt, ri), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat agn_aply_un_(atg *t, gen *g, te *an, err **e) {
    te *tn;
    if (ast_g_t(((te*) an->d[5].p)->d[4].p, &tn) != AST_STAT(OK)) return atg_err(t, an, e, "atg inv type for agn aply");
    if (tn->d[1].u4 == TYPE(VR)) return agn_vr_idx(t, g, an, e);
    return atg_err(t, an, e, "nyi");
}

void atg_dfn(atg *t) {
    atg_a_o(t, OC(DFN), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), dfn_i6_e_i6_s_i6);
    atg_a_o(t, OC(DFN), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6), dfnagn_e_e_);
    atg_a_o(t, OC(DFN), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(O), TYPE(I6), dfn_t_e_t_oa_t);
    atg_a_o(t, OC(DFN), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(A), TYPE(I6), dfn_t_e_t_oa_t);
    atg_a_o(t, OC(DFN), TYPE(U6), AST_CLS(E), TYPE(U6), AST_CLS(O), TYPE(U6), dfn_t_e_t_oa_t);
    atg_a_o(t, OC(DFN), TYPE(U6), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(U6), dfn_u6_e_u6_s_u6);
    atg_a_o(t, OC(DFN), TYPE(F6), AST_CLS(E), TYPE(F6), AST_CLS(S), TYPE(F6), dfn_f6_e_f6_s_f6);
    atg_a_o(t, OC(DFN), TYPE(FN), AST_CLS(E), TYPE(FN), AST_CLS(S), TYPE(_G), dfn_fn_e_fn_s__g);
    atg_a_o(t, OC(DFN), TYPE(NF), AST_CLS(E), TYPE(NF), AST_CLS(S), TYPE(_G), dfn_fn_e_fn_s__g);
    atg_a_o(t, OC(DFN), TYPE(F6), AST_CLS(E), TYPE(F6), AST_CLS(O), TYPE(F6), dfn_t_e_t_oa_t);
    atg_a_o(t, OC(DFN), TYPE(SG), AST_CLS(E), TYPE(SG), AST_CLS(A), TYPE(SG), dfn_t_e_t_oa_t);
    atg_a_o(t, OC(DFN), TYPE(SG), AST_CLS(E), TYPE(SG), AST_CLS(O), TYPE(SG), dfn_t_e_t_oa_t);
    atg_a_o(t, OC(DFN), TYPE(VR), AST_CLS(E), TYPE(VR), AST_CLS(A), TYPE(VR), dfn_t_e_t_oa_t);
    atg_a_o(t, OC(DFN), TYPE(UN), AST_CLS(E), TYPE(UN), AST_CLS(A), TYPE(UN), dfn_t_e_t_oa_t);
    atg_a_o(t, OC(DFN), TYPE(TE), AST_CLS(E), TYPE(TE), AST_CLS(V), TYPE(TE), dfn_tevrst_e_tevrst_vo_tevrst);
    atg_a_o(t, OC(DFN), TYPE(VR), AST_CLS(E), TYPE(VR), AST_CLS(V), TYPE(VR), dfn_tevrst_e_tevrst_vo_tevrst);
    atg_a_o(t, OC(DFN), TYPE(VR), AST_CLS(E), TYPE(VR), AST_CLS(O), TYPE(VR), dfn_tevrst_e_tevrst_vo_tevrst);
    atg_a_o(t, OC(DFN), TYPE(ST), AST_CLS(E), TYPE(ST), AST_CLS(O), TYPE(ST), dfn_tevrst_e_tevrst_vo_tevrst);
    atg_a_o(t, OC(DFN), TYPE(CJ), AST_CLS(E), TYPE(CJ), AST_CLS(A), TYPE(CJ), dfn_tevrst_e_tevrst_vo_tevrst);
    atg_a_o(t, OC(DFN), TYPE(FN), AST_CLS(E), TYPE(FN), AST_CLS(T), TYPE(FN), atg_nop);
    atg_a_o(t, OC(DFN), TYPE(UN), AST_CLS(E), TYPE(UN), AST_CLS(Z), TYPE(VD), dfn_un_e_un_z);
    atg_a_o(t, OC(DFN), TYPE(UN), AST_CLS(E), TYPE(UN), AST_CLS(E), TYPE(UN), dfn_un_e_un_e_un);
    atg_a_o(t, OC(AGN), TYPE(UN), AST_CLS(E), TYPE(UN), AST_CLS(Z), TYPE(VD), agn_un_e_un_z);
    atg_a_o(t, OC(AGN), TYPE(I6), AST_CLS(A), TYPE(I6), AST_CLS(O), TYPE(I6), agn_i6_a_i6_o_i6);
    atg_a_o(t, OC(AGN), TYPE(I6), AST_CLS(Z), TYPE(I6), AST_CLS(O), TYPE(I6), agn_i6_z_i6_o_i6);
    atg_a_o(t, OC(AGN), TYPE(ST), AST_CLS(Z), TYPE(ST), AST_CLS(O), TYPE(ST), agn_st_z_st_o_st);
    atg_a_o(t, OC(AGN), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6), dfnagn_e_e_);
    atg_a_o(t, OC(AGN), TYPE(F6), AST_CLS(E), TYPE(F6), AST_CLS(E), TYPE(F6), dfnagn_e_e_);
    atg_a_o(t, OC(AGN), TYPE(I6), AST_CLS(A), TYPE(UN), AST_CLS(S), TYPE(I6), agn_aply_un_);
    atg_a_o(t, OC(AGN), TYPE(I6), AST_CLS(A), TYPE(UN), AST_CLS(E), TYPE(I6), agn_aply_un_);
}
