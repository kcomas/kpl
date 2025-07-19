
#include "../chk.h"

static chk_stat chk_op_e_n_svoe_dfn_flg(chk *c, te *an, err **e, lte_flg lf) {
    te *lte = ((te*) an->d[5].p)->d[3].p, *rt;
    if (ast_g_t(an->d[6].p, &rt) != AST_STAT(OK)) return chk_err(c, an->d[6].p, e, "chk cannot get op r type");
    lte->d[2] = P(te_c(rt));
    ast_lst_tbl_e_s_f(lte, lf);
    an->d[3] = P(te_c(rt));
    return CHK_STAT(OK);
}

static chk_stat chk_op_e_n_svoea_dfn(chk *c, te *an, err **e) {
    return chk_op_e_n_svoe_dfn_flg(c, an, e, LTE_FLG(L));
}

static chk_stat chk_op_dfn_e_d(chk *c, te *an, err **e) {
    chk_stat stat;
    if ((stat = chk_op_e_n_svoe_dfn_flg(c, an, e, LTE_FLG(D))) != CHK_STAT(OK)) return stat;
    an->d[4].u4 = OC(NOP);
    return CHK_STAT(OK);
}

static chk_stat chk_op_dfn_e_t(chk *c, te *an, err **e) {
    return chk_op_e_n_svoe_dfn_flg(c, an, e, 0);
}

#define UN_TBL_S 5

static chk_stat chk_dfn_agn_un_e_z(chk *c, te *an, err **e) {
    te *lte = ((te*) an->d[5].p)->d[3].p, *z = an->d[6].p, *rt;
    if (!lte->d[2].p) {
        lte->d[2] = P(type_h_i(c->ta, NULL, TYPE(UN), tbl_i(c->tbla, tbl_mc_sdbm, tbl_mc_eq, lst_i(c->la, c->ta, (void*) te_f), te_i(UN_TBL_S, c->ta, NULL))));
        ast_lst_tbl_e_s_f(lte, LTE_FLG(L));
    }
    if (ast_g_t(z->d[4].p, &rt) != AST_STAT(OK)) return chk_err(c, an, e, "chk un type g inv");
    tbl *tt = ((te*) lte->d[2].p)->d[2].p;
    type_tbl_a(tt, c->ta, mc_c(z->d[5].p), 0, te_c(rt));
    lst_s(tt->i, type_h_cmp);
    an->d[3] = P(te_c(lte->d[2].p));
    return CHK_STAT(OK);
}

static chk_stat chk_op_dfn_sh(chk *c, te *an, err **e) {
    te *lte = ((te*) an->d[5].p)->d[3].p, *rt, *pn;
    if (!(ast_lst_tbl_e_g_f(lte) & LTE_FLG(O))) return chk_err(c, an, e, "chk redef of var");
    if (ast_g_t(an->d[6].p, &rt) != AST_STAT(OK)) return chk_err(c, an, e, "chk shadow type inv");
    if (!type_eq(lte->d[2].p, rt)) return chk_err(c, an, e, "chk shadow type neq");
    an->d[3] = P(te_c(rt));
    mc **s = (mc**) &lte->d[0].p;
    if (ast_g_pn(AST_CLS(L), an, &pn) != AST_STAT(OK)) return chk_err(c, an, e, "chk cannot get shadow parent lst");
    tbl *pt = chk_g_pn_lte_tbl(pn->d[0].p, *s);
    if (!pt) return chk_err(c, an, e, "chk shadow parent tbl inv");
    ast_lst_tbl_e_r_f(lte, LTE_FLG(O));
    mc_wf(s, '0');
    while (tbl_g_i(pt, P(*s), &pn) == TBL_STAT(OK)) mc_wf(s, '0');
    tbl_a(pt, te_c(lte));
    return CHK_STAT(OK);
}

static chk_stat chk_agn_a_o(chk *c, te *an, err **e) {
    chk_stat stat;
    te *lt, *rt;
    if (ast_g_t(((te*) an->d[5].p)->d[4].p, &lt) != AST_STAT(OK)) return chk_err(c, an, e, "chk agn rt inv");
    switch (lt->d[1].u4) {
        case TYPE(TE):
            if ((stat = a_te_g_rt(c, an->d[5].p, e, &lt)) != CHK_STAT(OK)) return stat;
            break;
        default:
            return chk_err(c, an, e, "chk inv aply tgt type");
    }
    if (ast_g_t(an->d[6].p, &rt) != AST_STAT(OK)) return chk_err(c, an, e, "chk agn rt inv");
    if (!type_eq(lt, rt)) return chk_err(c, an, e, "chk agn type neq");
    an->d[3] = P(te_c(rt));
    return CHK_STAT(OK);
}

static chk_stat chk_op_agn(chk *c, te *an, err **e) {
    te *lt, *rt;
    if (ast_g_t(an->d[5].p, &lt) != AST_STAT(OK)) return chk_err(c, an, e, "chk agn lt inv");
    if (ast_g_t(an->d[6].p, &rt) != AST_STAT(OK)) return chk_err(c, an, e, "chk agn rt inv");
    if (!type_eq(lt, rt)) return chk_err(c, an, e, "chk agn type neq");
    an->d[3] = P(te_c(rt));
    return CHK_STAT(OK);
}

static chk_stat chk_agn_a_un_(chk *c, te *an, err **e) {
    chk_stat stat;
    te *l = an->d[5].p, *lte;
    if ((stat = chk_fnnf_un_ret(c, l, e)) != CHK_STAT(OK)) return stat;
    lte = un_g_lte_ch(l->d[3].p, 'v');
    if (!lte) return chk_err(c, an, e, "chk inv un v type");
    an->d[3] = P(te_c(lte->d[2].p));
    return CHK_STAT(OK);
}

void chk_dfnagn(chk *c) {
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(S), TYPE(F6));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(O), TYPE(F6));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(O), TYPE(U6));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(O), TYPE(VR));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(V), TYPE(TE));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(O), TYPE(ST));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(A), TYPE(I6));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(A), TYPE(SG));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(O), TYPE(SG));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(A), TYPE(UN));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(A), TYPE(CJ));
    CHK_AA(c, chk_op_dfn_sh, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_dfn_e_d, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(S), TYPE(ET));
    CHK_AA(c, chk_op_dfn_e_t, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(T), TYPE(FN));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(FN), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(FN), AST_CLS(O), TYPE(FN));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(NF), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(NF), AST_CLS(O), TYPE(NF));
    CHK_AA(c, chk_dfn_agn_un_e_z, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(Z), TYPE(VD));
    CHK_AA(c, chk_dfn_agn_un_e_z, AST_CLS(O), TYPE(_N), OC(AGN), TYPE(_A), AST_CLS(E), TYPE(UN), AST_CLS(Z), TYPE(VD));
    CHK_AA(c, chk_agn_a_o, AST_CLS(O), TYPE(_N), OC(AGN), TYPE(_A), AST_CLS(A), TYPE(I6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_l_z_r, AST_CLS(O), TYPE(_N), OC(AGN), TYPE(_A), AST_CLS(Z), TYPE(I6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_agn, AST_CLS(O), TYPE(_N), OC(AGN), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_op_agn, AST_CLS(O), TYPE(_N), OC(AGN), TYPE(_A), AST_CLS(E), TYPE(F6), AST_CLS(E), TYPE(F6));
    CHK_AA(c, chk_agn_a_un_, AST_CLS(O), TYPE(_N), OC(AGN), TYPE(_A), AST_CLS(A), TYPE(UN), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_agn_a_un_, AST_CLS(O), TYPE(_N), OC(AGN), TYPE(_A), AST_CLS(A), TYPE(UN), AST_CLS(E), TYPE(I6));
}
