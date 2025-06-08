
#include "fld.h"

static fld_stat err(fld_stat stat, te *an, te **e) {
    *e = an;
    return stat;
}

static fld_stat aply_op_r(fld *f, te **an, te **e) {
    (void) f;
    lst *l = (*an)->d[5].p;
    (*an)->d[5] = P(NULL);
    if (l->l < 1 || l-l > 2) return err(FLD_STAT(INV), *an, e);
    te *on = (*an)->d[4].p;
    on->d[0] = (*an)->d[0];
    (*an)->d[0] = (*an)->d[1] = (*an)->d[4] = P(NULL);
    te_f(*an);
    un a = P(NULL), b = P(NULL);
    lst_sb(l, &b);
    if (l->l == 1) lst_sb(l, &a);
    lst_f(l);
    on->d[5] = a;
    on->d[6] = b;
    *an = on;
    return FLD_STAT(OK);
}

static bool aply_op_t(const te *an) {
    if (an->d[4].p && ((te*) an->d[4].p)->d[2].u6 == AST_CLS(O)) return true;
    return false;
}

fld *fld_b(fld *f) {
    fld_a(f, AST_CLS(A), aply_op_t, aply_op_r);
    return f;
}
