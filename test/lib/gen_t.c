
#include "gen_t.h"

lst *gen_mklst(void) {
    return lst_i(&malloc, &malloc, &free, (void*) &te_f, &free);
}

tbl *gen_cls_info_tbl(void) {
    lst *tl = gen_mklst();
    te *b = te_i(GEN_CLS(L) + 1 + X64_TYPE(F6) + 1, &malloc, &free);
    tbl *t = tbl_i(&malloc, &free, &tbl_no_hsh, &tbl_un_eq, tl, b);
    return t;
}

tbl *gen_op_tbl(size_t bcks) {
    lst *tl = gen_mklst();
    te *b = te_i(bcks, &malloc, &free);
    tbl *t = tbl_i(&malloc, &free, &tbl_no_hsh, &tbl_un_eq, tl, b);
    return t;
}

te *gen_arg(x64_type t, size_t id) {
    return gen_var(&malloc, &free, GEN_CLS(A), U3(t), id);
}

void gen_entry_f(void *p) {
    te *t = (te*) p;
    if (t->l == 3) tbl_f(t->d[2].p);
    else tbl_f(t->d[4].p);
    free(t);
}

void code_entry_f(void *p) {
    te *t = (te*) p;
    te_f(t->d[1].p);
    te_f(t->d[2].p);
    te_f(t->d[3].p);
    free(t);
}
