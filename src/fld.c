
#include "fld.h"

static fld_stat aply_op_r(fld *f, te **an) {
    // TODO convert aply op to op
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
