
#include "as_t.h"

const alfr am = { .a = malloc, .f = free };

uint8_t *m = NULL;

as *ba = NULL;

static __attribute__((constructor(101))) void as_c(void) {
    m = x64_mmap(1e6);
    ba = as_b(as_i(&am, &al_te, &al_lst, &am, as_x64_err_g_p, as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
}

static __attribute__((destructor)) void as_d(void) {
    x64_munmap(1e6, m);
    as_f(ba);
}

static te *as_arg_v(as *a, arg_id id, un v) {
    return as_arg_i(a, id, v);
}

te *as_arg_r(as *a, size_t rid) {
    return as_arg_v(a, rid < XMM(0) ? ARG_ID(R) : ARG_ID(X), U3(rid));
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
    return lst_i(&al_lst, &al_te, (void*) te_f);
}

tbl *as_arg_tbl(void) {
    return as_op_tbl(6);
}

tbl *as_op_tbl(size_t bcks) {
    lst *tl = as_mklst();
    te *b = te_i(bcks, &al_te, NULL);
    tbl *t = tbl_i(&al_tbl, tbl_no_hsh, tbl_un_eq, tl, b);
    return t;
}
