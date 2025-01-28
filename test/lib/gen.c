
#include "gen_t.h"

int main(void) {
    gen *g = gen_i(&malloc, &free, &gen_entry_f, &code_entry_f, &gen_cls_info_tbl, gen_op_tbl(10/*TODO*/), gen_mklst());
    gen_b(g);
    gen_op_p(g->oci, false, 0);
    gen_a(g, GEN_OP(ADD), gen_arg(X64_TYPE(U6), 0),  gen_arg(X64_TYPE(U6), 0), gen_arg(X64_TYPE(U6), 1));
    gen_a(g, GEN_OP(RET), gen_arg(X64_TYPE(U6), 0), NULL, NULL);
    gen_p(g, NULL);
    gen_f(g);
    return 0;
}
