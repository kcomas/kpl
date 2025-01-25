
#include "gen_t.h"

lst *gen_mklst(void) {
    return lst_i(&malloc, &malloc, &free, (void*) &te_f, &free);
}

static size_t cls_type_hsh(un d) {
    te *t = (te*) d.p;
    return t->d[0].u6 + t->d[1].u6;
}

static bool cls_type_eq(un a, un b) {
    te *ta = (te*) a.p;
    te *tb = (te*) b.p;
    return ta->d[0].u6 == tb->d[0].u6 && tb->d[1].u6 == tb->d[1].u6;
}

tbl *gen_cls_type_tbl(void) {
    lst *tl = gen_mklst();
    te *b = te_i((GEN_CLS(L) + 1) * 2, &malloc, &free);
    tbl *t = tbl_i(&malloc, &free, &cls_type_hsh, &cls_type_eq, tl, b);
    return t;
}

static size_t no_hsh(un d) {
    return d.u6;
}

static bool id_eq(un a, un b) {
    return a.u6 == b.u6;
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
    else tbl_f(t->d[3].p);
    free(t);
}
