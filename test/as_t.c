
#include "as_t.h"

const alfr am = { .a = &malloc, .f = &free };

static te *as_arg_v(as *a, arg_id id, un v) {
    return as_arg_i(a, id, v);
}

te *as_arg_r(as *a, size_t rid) {
    return as_arg_v(a, ARG_ID(R), U3(rid));
}

te *as_arg_rm(as *a, size_t rmid) {
    return as_arg_v(a, ARG_ID(RM), U3(rmid));
}

te *as_arg_l(as *a, size_t lid) {
    return as_arg_v(a, ARG_ID(L), U6(lid));
}

te *as_arg_qw(as *a, un v) {
    return as_arg_v(a, ARG_ID(QW), v);
}

te *as_arg_b(as *a, uint8_t b) {
    return as_arg_v(a, ARG_ID(B), U3(b));
}

lst *as_mklst(void) {
    return lst_i(&am, &am, (void*) &te_f);
}

tbl *as_arg_tbl(void) {
    return as_op_tbl(6);
}

tbl *as_op_tbl(size_t bcks) {
    lst *tl = as_mklst();
    te *b = te_i(bcks, &am, NULL);
    tbl *t = tbl_i(&am, &tbl_no_hsh, &tbl_un_eq, tl, b);
    return t;
}

void as_label_entry_f(void *p) {
    te *l = p;
    te_f(l->d[1].p);
    lst_f(l->d[2].p);
    l->af->f(l);
}

void as_op_entry_f(void *p) {
    te *oe = p;
    tbl_f(oe->d[3].p);
    oe->af->f(oe);
}

void as_code_entry_f(void *p) {
    te *o = p;
    te_f(o->d[2].p);
    te_f(o->d[3].p);
    te_f(o->d[4].p);
    te_f(o->d[5].p);
    o->af->f(o);
}
