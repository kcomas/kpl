
#include "gen_t.h"

lst *gen_mklst(void) {
    return lst_i(&malloc, &malloc, &free, (void*) &te_f, &free);
}

static size_t no_hsh(un d) {
    return d.u6;
}

static bool id_eq(un a, un b) {
    return a.u6 == b.u6;
}

tbl *gen_cls_info_tbl(void) {
    lst *tl = gen_mklst();
    te *b = te_i(GEN_CLS(L) + 1 + X64_TYPE(F6) + 1, &malloc, &free);
    tbl *t = tbl_i(&malloc, &free, &no_hsh, &id_eq, tl, b);
    return t;
}

tbl *gen_op_tbl(size_t bcks) {
    lst *tl = gen_mklst();
    te *b = te_i(bcks, &malloc, &free);
    tbl *t = tbl_i(&malloc, &free, &no_hsh, &id_eq, tl, b);
    return t;
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
