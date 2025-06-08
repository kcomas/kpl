
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

void gen_call_m_f(void *p) {
    te *t = p;
    vr_f(t->d[1].p);
    t->af->f(t);
}

te *gen_call_m(gen *g, size_t n, ...) {
    vr *v = vr_i(n, g->af, (void*) te_f);
    va_list args;
    va_start(args, n);
    while (n > 0) {
        vr_ab(&v, P(va_arg(args, te*)));
        n--;
    }
    va_end(args);
    return gen_var_i(g, gen_call_m_f, GEN_CLS(M), X64_TYPE(N), P(v));
}

te *gen_lbl(gen *g, size_t id) {
    return gen_var_i(g, NULL, GEN_CLS(L), X64_TYPE(N), U6(id));
}

te *gen_arg(gen *g, x64_type t, size_t id) {
    return gen_var_i(g, NULL, GEN_CLS(A), t, U6(id));
}

te *gen_tmp(gen *g, x64_type t, size_t id) {
    return gen_var_i(g, NULL, GEN_CLS(T), t, U6(id));
}

te *gen_data(gen *g, x64_type t, un d) {
    return gen_var_i(g, NULL, GEN_CLS(D), t, d);
}
