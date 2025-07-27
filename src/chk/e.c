
#include "../chk.h"

static chk_stat chk_e_(chk *c, te *an, err **e) {
    (void) c;
    (void) e;
    te* lte = an->d[3].p;
    lte = chk_g_pn_lte(an, lte->d[0].p);
    if (lte) {
        te_f(an->d[3].p);
        an->d[3] = P(te_c(lte));
    }
    return CHK_STAT(OK);
}

void chk_entry(chk *c) {
    CHK_AA(c, chk_e_, AST_CLS(E), TYPE(_N), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(E), TYPE(I6), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(E), TYPE(U6), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(E), TYPE(F6), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(E), TYPE(SG), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(E), TYPE(FN), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(E), TYPE(NF), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(E), TYPE(CJ), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(E), TYPE(TE), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(E), TYPE(ST), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(E), TYPE(ET), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(E), TYPE(VR), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(E), TYPE(UN), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(E), TYPE(ER), AST_CLS(_), TYPE(_A));
}
