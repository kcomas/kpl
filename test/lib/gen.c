
#include "gen_t.h"

int main(void) {
    gen *g = gen_i(&malloc, &free, &gen_entry_f, &gen_code_entry_f, &gen_cls_info_tbl, gen_op_tbl(10/*TODO*/), gen_mklst());
    gen_b(g);
    gen_op_p(g->oci, false, 0);
    gen_a(g, GEN_OP(ADD), gen_arg(X64_TYPE(U6), 0),  gen_arg(X64_TYPE(U6), 0), gen_arg(X64_TYPE(U6), 1));
    gen_a(g, GEN_OP(RET), gen_arg(X64_TYPE(U6), 0), NULL, NULL);
    gen_p(g, NULL);
    gen_st *st = gen_st_i(&malloc, &free, gen_op_tbl(20), gen_op_tbl(20), vr_i(16, &malloc, NULL, &free), vr_i(16, &malloc, NULL, &free));
    as *a = as_b(as_i(&malloc, &free, &as_label_entry_f, &as_op_entry_f, &as_code_entry_f, &as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
    if (gen_st_p1(g, st, a) != GEN_STAT(OK)) exit(33);
    gen_st_f(st);
    gen_f(g);
    as_f(a);
    return 0;
}
