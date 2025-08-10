
#include "../chk.h"

static chk_stat chk_e(chk *c, te *an, err **e) {
    te* lte = an->d[3].p;
    if (ast_lst_tbl_e_g_f(lte) & LTE_FLG(M)) return chk_err(c, an, e, "chk moved");
    if (lte->d[2].p) return CHK_STAT(OK);
    lte = chk_g_pn_lte(an, lte->d[0].p);
    if (lte) {
        te_f(an->d[3].p);
        an->d[3] = P(te_c(lte));
    }
    return CHK_STAT(OK);
}

void chk_entry(chk *c) {
    CHK_AA(c, chk_e, AST_CLS(E), TYPE(_N), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_e, AST_CLS(E), TYPE(I6), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_e, AST_CLS(E), TYPE(U6), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_e, AST_CLS(E), TYPE(F6), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_e, AST_CLS(E), TYPE(SG), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_e, AST_CLS(E), TYPE(FN), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_e, AST_CLS(E), TYPE(NF), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_e, AST_CLS(E), TYPE(CJ), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_e, AST_CLS(E), TYPE(TE), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_e, AST_CLS(E), TYPE(ST), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_e, AST_CLS(E), TYPE(ET), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_e, AST_CLS(E), TYPE(VR), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_e, AST_CLS(E), TYPE(UN), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_e, AST_CLS(E), TYPE(ER), AST_CLS(_), TYPE(_A));
}
