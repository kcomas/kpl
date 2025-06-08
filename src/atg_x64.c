
#include "atg_x64.h"

static bool lst_cst_t(const te *an) {
    return an->d[4].u4 == OC(CST) && an->d[6].p && ((te*) an->d[6].p)->d[2].u4 == AST_CLS(L);
}

bool atg_x64_enq(const te *an) {
    switch (an->d[2].u4) {
        case AST_CLS(O):
            return lst_cst_t(an);
        case AST_CLS(L):
            return an->d[0].p && ((te*) an->d[0].p)->d[2].u4 == AST_CLS(R);
        default:
            break;
    }
    return false;
}

atg_stat atg_err(atg_stat stat, te *an, te **e) {
    *e = te_c(an);
    return stat;
}

static atg_stat lst_cst_s(atg *t, gen *g, te *restrict rn, te *restrict an, te **e) {
    (void) t;
    (void) an;
    if (gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL) != GEN_STAT(OK)) return atg_err(ATG_STAT(INV), rn, e);
    return ATG_STAT(OK);
}

static atg_stat lst_cst_e(atg *t, gen *g, te *restrict rn, te *restrict an, te **e) {
    (void) t;
    (void) an;
    (void) rn;
    (void) g;
    (void) e;
    HERE("TODO");
    return ATG_STAT(OK);
}

atg *atg_b(atg *t) {
    atg_a_se(t, lst_cst_t, lst_cst_s, lst_cst_e);
    return t;
}
