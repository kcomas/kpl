
#include "gen_t.h"

#define A(stmt) if (stmt != GEN_STAT(OK)) exit(22)

void btest(void) {
    printf(">>>> BTEST\n");
    gen *g = gen_i(&malloc, &free, &gen_entry_f, &gen_code_entry_f, &gen_cls_info_tbl, gen_op_tbl(10/*TODO*/), gen_mklst());
    gen_b(g);
    gen_op_p(g->oci, false, 0);
    A(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    A(gen_a(g, GEN_OP(ADD), gen_arg(X64_TYPE(U6), 0),  gen_arg(X64_TYPE(U6), 0), gen_arg(X64_TYPE(U6), 1)));
    A(gen_a(g, GEN_OP(LEAVE), gen_arg(X64_TYPE(U6), 0), NULL, NULL));
    gen_st *st = gen_st_i(&malloc, &free, &gen_st_atmf, &gen_st_latf, gen_op_tbl(20), gen_op_tbl(20), vr_i(16, &malloc, NULL, &free), vr_i(16, &malloc, NULL, &free));
    as *a = as_b(as_i(&malloc, &free, &as_label_entry_f, &as_op_entry_f, &as_code_entry_f, &as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
    if (gen_st_p1(g, st) != GEN_STAT(OK)) exit(33);
    gen_st_p(st);
    if (gen_n(&malloc, &free, g, st, a) != GEN_STAT(OK)) exit(44);
    printf("STATE AFTER\n");
    gen_st_p(st);
    uint8_t *m = x64_mmap(1);
    if (as_n(a, m) != AS_STAT(OK)) exit(55);
    gen_p(g, m);
    uint64_t arg1 = 3, arg2 = 4;
    printf("Add(%lu, %lu): %lu\n", arg1, arg2, ((uint64_t(*)(uint64_t, uint64_t)) m)(arg1, arg2));
    gen_st_f(st);
    gen_f(g);
    as_f(a);
    x64_munmap(1, m);
    printf("<<<< BTEST\n");
}

void fibtest(void) {
    printf(">>>> FIBTEST\n");
    gen *g = gen_i(&malloc, &free, &gen_entry_f, &gen_code_entry_f, &gen_cls_info_tbl, gen_op_tbl(10/*TODO*/), gen_mklst());
    gen_b(g);
    gen_op_p(g->oci, false, 0);
    A(gen_a(g, GEN_OP(LBL), gen_lbl(1), NULL, NULL));
    A(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    A(gen_a(g, GEN_OP(NE), gen_arg(X64_TYPE(U6), 0), gen_data(X64_TYPE(U6), U6(0)), gen_lbl(2)));
    A(gen_a(g, GEN_OP(LEAVE), gen_data(X64_TYPE(U6), U6(0)), NULL, NULL));
    A(gen_a(g, GEN_OP(LBL), gen_lbl(2), NULL, NULL));
    A(gen_a(g, GEN_OP(ULTE), gen_arg(X64_TYPE(U6), 0), gen_data(X64_TYPE(U6), U6(2)), gen_lbl(3)));
    A(gen_a(g, GEN_OP(LEAVE), gen_data(X64_TYPE(U6), U6(1)), NULL, NULL));
    A(gen_a(g, GEN_OP(LBL), gen_lbl(3), NULL, NULL));
    A(gen_a(g, GEN_OP(LEAVE), gen_arg(X64_TYPE(U6), 0), NULL, NULL));
    gen_st *st = gen_st_i(&malloc, &free, &gen_st_atmf, &gen_st_latf, gen_op_tbl(20), gen_op_tbl(20), vr_i(16, &malloc, NULL, &free), vr_i(16, &malloc, NULL, &free));
    as *a = as_b(as_i(&malloc, &free, &as_label_entry_f, &as_op_entry_f, &as_code_entry_f, &as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
    if (gen_st_p1(g, st) != GEN_STAT(OK)) exit(33);
    gen_st_p(st);
    if (gen_n(&malloc, &free, g, st, a) != GEN_STAT(OK)) exit(44);
    printf("STATE AFTER\n");
    gen_st_p(st);
    uint8_t *m = x64_mmap(1);
    if (as_n(a, m) != AS_STAT(OK)) exit(55);
    gen_p(g, m);
    uint64_t n = 35;
    printf("Fib(%lu): %lu\n", n, ((uint64_t(*)(uint64_t)) m)(n));
    gen_st_f(st);
    gen_f(g);
    as_f(a);
    x64_munmap(1, m);
    printf("<<<< FIBTEST\n");
}

int main(void) {
    btest();
    fibtest();
    return 0;
}
