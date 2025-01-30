
#include "as_t.h"

static te *as_arg_v(arg_id id, un v) {
    return as_arg(&malloc, &free, id, v);
}

te *as_arg_r(size_t rid) {
    return as_arg_v(ARG_ID(R), U3(rid));
}

te *as_arg_rm(size_t rmid) {
    return as_arg_v(ARG_ID(RM), U3(rmid));
}

te *as_arg_l(size_t lid) {
    return as_arg_v(ARG_ID(L), U6(lid));
}

te *as_arg_qw(un v) {
    return as_arg_v(ARG_ID(QW), v);
}

te *as_arg_b(uint8_t b) {
    return as_arg_v(ARG_ID(B), U3(b));
}

lst *as_mklst(void) {
    return lst_i(&malloc, &malloc, &free, (void*) &te_f, &free);
}

tbl *as_arg_tbl(void) {
    return as_op_tbl(6);
}

tbl *as_op_tbl(size_t bcks) {
    lst *tl = as_mklst();
    te *b = te_i(bcks, &malloc, &free);
    tbl *t = tbl_i(&malloc, &free, &tbl_no_hsh, &tbl_un_eq, tl, b);
    return t;
}

void as_label_entry_f(void *p) {
    te *l = (te*) p;
    te_f(l->d[1].p);
    lst_f(l->d[2].p);
    free(l);
}

void as_op_entry_f(void *p) {
    te *oe = (te*) p;
    tbl_f(oe->d[3].p);
    free(oe);
}

void as_code_entry_f(void *p) {
    te *o = (te*) p;
    te_f(o->d[2].p);
    te_f(o->d[3].p);
    te_f(o->d[4].p);
    te_f(o->d[5].p);
    free(o);
}
