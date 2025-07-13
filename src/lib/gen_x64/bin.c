
#include "../gen_x64.h"

static gen_stat and_auauau_fn(gen *g, void *s, te *ci, as *a, err **e) {
    (void) s;
    (void) a;
    return gen_err(g, ci, e, "TODO and");
}

void gen_bin(gen *g) {
    GEN_OP_A3(g, GEN_OP(AND), GEN_CLS(T), X64_TYPE(U3), GEN_CLS(T), X64_TYPE(U3), GEN_CLS(T), X64_TYPE(U3), and_auauau_fn);
}
