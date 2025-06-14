
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

static chk_stat chk_op_dfn_d(chk *c, te *an, err **e) {
    chk_stat stat;
    if ((stat = chk_op_e_n_svoe_dfn_flg(c, an, e, LTE_FLG(D))) != CHK_STAT(OK)) return stat;
    an->d[4].u4 = OC(NOP);
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

void chk_dfnagn(chk *c) {
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(S), TYPE(F6));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(O), TYPE(U6));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(O), TYPE(VR));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(V), TYPE(TE));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(O), TYPE(ST));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(A), TYPE(I6));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(A), TYPE(SG));
    CHK_AA(c, chk_op_e_n_svoea_dfn, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(T), TYPE(FN));
    CHK_AA(c, chk_op_dfn_d, AST_CLS(O), TYPE(_N), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(_N), AST_CLS(S), TYPE(ET));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(FN), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(FN), AST_CLS(O), TYPE(FN));
    CHK_AA(c, chk_nop, AST_CLS(O), TYPE(NF), OC(DFN), TYPE(_A), AST_CLS(E), TYPE(NF), AST_CLS(O), TYPE(NF));
    CHK_AA(c, chk_agn_a_o, AST_CLS(O), TYPE(_N), OC(AGN), TYPE(_A), AST_CLS(A), TYPE(I6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_l_z_r, AST_CLS(O), TYPE(_N), OC(AGN), TYPE(_A), AST_CLS(Z), TYPE(I6), AST_CLS(O), TYPE(I6));
}
