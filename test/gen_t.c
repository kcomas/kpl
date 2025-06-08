
#include "gen_t.h"

extern const alfr am;

lst *gen_mklst(void) {
    return lst_i(&am, &am, (void*) te_f);
}

tbl *gen_cls_info_tbl(void) {
    lst *tl = gen_mklst();
    te *b = te_i(GEN_CLS(L) + 1 + X64_TYPE(F6) + 1, &am, NULL);
    tbl *t = tbl_i(&am, tbl_no_hsh, tbl_un_eq, tl, b);
    return t;
}

tbl *gen_op_tbl(size_t bcks) {
    lst *tl = gen_mklst();
    te *b = te_i(bcks, &am, NULL);
    tbl *t = tbl_i(&am, tbl_no_hsh, tbl_un_eq, tl, b);
    return t;
}
