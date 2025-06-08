
#include "fld.h"

static fld_stat err(fld_stat stat, te *an, te **e) {
    *e = te_c(an);
    return stat;
}

static fld_stat aply_op_r(fld *f, te **an, te **e) {
    (void) f;
    lst *l = (*an)->d[5].p;
    if (l->l < 1 || l-l > 2) return err(FLD_STAT(INV), *an, e);
    te *on = (*an)->d[4].p;
    on->d[0] = (*an)->d[0];
    (*an)->d[0] = (*an)->d[1] = (*an)->d[4] = (*an)->d[5] = P(NULL);
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
    return an->d[2].u6 == AST_CLS(A) && an->d[4].p && ((te*) an->d[4].p)->d[2].u6 == AST_CLS(O);
}

static fld_stat aply_type_r(fld *f, te **an, te **e) {
    te *tn = (*an)->d[4].p;
    type_cls tc = type_g_c(((te*) tn->d[3].p)->d[0].u6);
    switch (tc) {
        default:
            return err(FLD_STAT(INV), *an, e);
    }
    return FLD_STAT(OK);
}

static bool aply_type_t(const te *an) {
    return an->d[2].u6 == AST_CLS(A) && an->d[4].p && ((te*) an->d[4].p)->d[2].u6 == AST_CLS(T);
}

fld *fld_b(fld *f) {
    fld_a(f, AST_CLS(A), aply_op_t, aply_op_r);
    fld_a(f, AST_CLS(A), aply_type_t, aply_type_r);
    return f;
}
