
#include "gen_t.h"

const alfr gm = { .a = &malloc, .f = &free };

lst *gen_mklst(void) {
    return lst_i(&gm, &gm, (void*) &te_f);
}

tbl *gen_cls_info_tbl(void) {
    lst *tl = gen_mklst();
    te *b = te_i(GEN_CLS(L) + 1 + X64_TYPE(F6) + 1, &gm, NULL);
    tbl *t = tbl_i(&gm, &tbl_no_hsh, &tbl_un_eq, tl, b);
    return t;
}

tbl *gen_op_tbl(size_t bcks) {
    lst *tl = gen_mklst();
    te *b = te_i(bcks, &gm, NULL);
    tbl *t = tbl_i(&gm, &tbl_no_hsh, &tbl_un_eq, tl, b);
    return t;
}

void gen_call_m_f(void *p) {
    te *t = p;
    vr_f(t->d[2].p);
    t->af->f(t);
}

te *gen_call_m(size_t n, ...) {
    vr *v = vr_i(n, &gm, (void*) &te_f);
    va_list args;
    va_start(args, n);
    while (n > 0) {
        vr_ab(&v, P(va_arg(args, te*)));
        n--;
    }
    va_end(args);
    return gen_var(&gm, &gen_call_m_f, GEN_CLS(M), U3(X64_TYPE(N)), P(v));
}

te *gen_lbl(size_t id) {
    return gen_var(&gm, NULL, GEN_CLS(L), U3(X64_TYPE(N)), U6(id));
}

te *gen_arg(x64_type t, size_t id) {
    return gen_var(&gm, NULL, GEN_CLS(A), U3(t), U6(id));
}

te *gen_tmp(x64_type t, size_t id) {
    return gen_var(&gm, NULL, GEN_CLS(T), U3(t), U6(id));
}

te *gen_data(x64_type t, un d) {
    return gen_var(&gm, NULL, GEN_CLS(D), U3(t), d);
}

void gen_entry_f(void *p) {
    te *t = p;
    if (t->l == 3) tbl_f(t->d[2].p);
    else tbl_f(t->d[4].p);
    t->af->f(t);
}

void gen_code_entry_f(void *p) {
    te *t = p;
    te_f(t->d[1].p);
    te_f(t->d[2].p);
    te_f(t->d[3].p);
    te_f(t->d[5].p);
    te_f(t->d[6].p);
    t->af->f(t);
}

void gen_st_atmf(void *p) {
    te *t = p;
    te_f(t->d[1].p);
    t->af->f(t);
}

void gen_st_latf(void *p) {
    te *t = p;
    te_f(t->d[1].p);
    te_f(t->d[2].p);
    t->af->f(t);
}
