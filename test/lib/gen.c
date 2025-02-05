
#include "gen_t.h"

extern const alfr gm;

#define A(stmt) if (stmt != GEN_STAT(OK)) exit(22)

void btest(void) {
    printf(">>>> BTEST\n");
    gen *g = gen_i(&gm, &gm, &gen_entry_f, &gen_code_entry_f, &gen_cls_info_tbl, gen_op_tbl(10/*TODO*/), gen_mklst());
    gen_b(g);
    gen_op_p(g->oci, false, 0);
    A(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    A(gen_a(g, GEN_OP(ADD), gen_arg(g, X64_TYPE(U6), 0),  gen_arg(g, X64_TYPE(U6), 0), gen_arg(g, X64_TYPE(U6), 1)));
    A(gen_a(g, GEN_OP(LEAVE), gen_arg(g, X64_TYPE(U6), 0), NULL, NULL));
    gen_st *st = gen_st_i(&gm, &gm, &gen_st_atmf, &gen_st_latf, gen_op_tbl(20), gen_op_tbl(20), vr_i(16, &gm, NULL), vr_i(16, &gm, NULL));
    as *a = as_b(as_i(&gm, &gm, &gm, &as_label_entry_f, &as_op_entry_f, &as_code_entry_f, &as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
    if (gen_st_p1(g, st) != GEN_STAT(OK)) exit(33);
    gen_st_p(st);
    if (gen_n(g, st, a) != GEN_STAT(OK)) exit(44);
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

static gen *init(void) {
    gen *g = gen_i(&gm, &gm, &gen_entry_f, &gen_code_entry_f, &gen_cls_info_tbl, gen_op_tbl(10/*TODO*/), gen_mklst());
    gen_b(g);
    return g;
}

static void build(gen *g, uint8_t *m) {
    gen_st *st = gen_st_i(&gm, &gm, &gen_st_atmf, &gen_st_latf, gen_op_tbl(20), gen_op_tbl(20), vr_i(16, &gm, NULL), vr_i(16, &gm, NULL));
    as *a = as_b(as_i(&gm, &gm, &gm, &as_label_entry_f, &as_op_entry_f, &as_code_entry_f, &as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
    if (gen_st_p1(g, st) != GEN_STAT(OK)) exit(33);
    gen_st_p(st);
    if (gen_n(g, st, a) != GEN_STAT(OK)) exit(44);
    printf("STATE AFTER\n");
    gen_st_p(st);
    if (as_n(a, m) != AS_STAT(OK)) exit(55);
    gen_p(g, m);
    gen_st_f(st);
    gen_f(g);
    as_f(a);
}

void fibtest(void) {
    printf(">>>> FIBTEST\n");
    gen *g = init();
    A(gen_a(g, GEN_OP(LBL), gen_lbl(g, 0), NULL, NULL));
    A(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    A(gen_a(g, GEN_OP(NE), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U6), U6(0)), gen_lbl(g, 1)));
    A(gen_a(g, GEN_OP(LEAVE), gen_data(g, X64_TYPE(U6), U6(0)), NULL, NULL));
    A(gen_a(g, GEN_OP(LBL), gen_lbl(g, 1), NULL, NULL));
    A(gen_a(g, GEN_OP(GT), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U6), U6(2)), gen_lbl(g, 2)));
    A(gen_a(g, GEN_OP(LEAVE), gen_data(g, X64_TYPE(U6), U6(1)), NULL, NULL));
    A(gen_a(g, GEN_OP(LBL), gen_lbl(g, 2), NULL, NULL));
    A(gen_a(g, GEN_OP(SUB), gen_tmp(g, X64_TYPE(U6), 0), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U3), U3(1))));
    A(gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(U6), 0), gen_call_m(g, 1, gen_tmp(g, X64_TYPE(U6), 0)), gen_lbl(g, 0)));
    A(gen_a(g, GEN_OP(SUB), gen_tmp(g, X64_TYPE(U6), 1), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U3), U3(2))));
    A(gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(U6), 1), gen_call_m(g, 1, gen_tmp(g, X64_TYPE(U6), 1)), gen_lbl(g, 0)));
    A(gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(U6), 0), gen_tmp(g, X64_TYPE(U6), 0), gen_tmp(g, X64_TYPE(U6), 1)));
    A(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(U6), 0), NULL, NULL));
    uint8_t *m = x64_mmap(1);
    build(g, m);
    uint64_t n = 8;
    //uint64_t n = 35;
    printf("Fib(%lu): %lu\n", n, ((uint64_t(*)(uint64_t)) m)(n));
    x64_munmap(1, m);
    printf("<<<< FIBTEST\n");
}

void acktest(void) {
    printf(">>>> ACKTEST\n");
    gen *g = init();
    A(gen_a(g, GEN_OP(LBL), gen_lbl(g, 0), NULL, NULL));
    A(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    // m == 0
    A(gen_a(g, GEN_OP(NE), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U6), U6(0)), gen_lbl(g, 1)));
    A(gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(U6), 0), gen_arg(g, X64_TYPE(U6), 1), gen_data(g, X64_TYPE(U3), U3(1))));
    A(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(U6), 0), NULL, NULL));
    // m > 0 && n == 0
    A(gen_a(g, GEN_OP(LBL), gen_lbl(g, 1), NULL, NULL));
    A(gen_a(g, GEN_OP(EQ), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U6), U6(0)), gen_lbl(g, 2)));
    A(gen_a(g, GEN_OP(NE), gen_arg(g, X64_TYPE(U6), 1), gen_data(g, X64_TYPE(U6), U6(0)), gen_lbl(g, 2)));
    A(gen_a(g, GEN_OP(SUB), gen_tmp(g, X64_TYPE(U6), 0), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U3), U3(1))));
    // call ack(m - 1, 1)
    A(gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(U6), 0), gen_call_m(g, 2, gen_tmp(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U6), U6(1))), gen_lbl(g, 0)));
    A(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(U6), 0), NULL, NULL));
    // m > 0 && n > 0
    A(gen_a(g, GEN_OP(LBL), gen_lbl(g, 2), NULL, NULL));
    A(gen_a(g, GEN_OP(EQ), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U6), U6(0)), gen_lbl(g, 3)));
    A(gen_a(g, GEN_OP(EQ), gen_arg(g, X64_TYPE(U6), 1), gen_data(g, X64_TYPE(U6), U6(0)), gen_lbl(g, 3)));
    // call ack(m - 1, ack(m, n - 1))
    A(gen_a(g, GEN_OP(SUB), gen_tmp(g, X64_TYPE(U6), 1), gen_arg(g, X64_TYPE(U6), 1), gen_data(g, X64_TYPE(U3), U3(1))));
    A(gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(U6), 1), gen_call_m(g, 2, gen_arg(g, X64_TYPE(U6), 0), gen_tmp(g, X64_TYPE(U6), 1)), gen_lbl(g, 0)));
    A(gen_a(g, GEN_OP(SUB), gen_tmp(g, X64_TYPE(U6), 2), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U3), U3(1))));
    A(gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(U6), 2), gen_call_m(g, 2, gen_tmp(g, X64_TYPE(U6), 2), gen_tmp(g, X64_TYPE(U6), 1)), gen_lbl(g, 0)));
    A(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(U6), 2), NULL, NULL));
    // else
    A(gen_a(g, GEN_OP(LBL), gen_lbl(g, 3), NULL, NULL));
    A(gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(U6), 3), gen_arg(g, X64_TYPE(U6), 1), gen_data(g, X64_TYPE(U3), U3(1))));
    A(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(U6), 3), NULL, NULL));
    uint8_t *x = x64_mmap(1);
    build(g, x);
    uint64_t m = 2, n = 4;
    //uint64_t m = 3, n = 10;
    printf("Ack(%lu, %lu): %lu\n", m, n, ((uint64_t(*)(uint64_t, uint64_t)) x)(m, n));
    x64_munmap(1, x);
    printf("<<<< ACKTEST\n");
}

int main(void) {
    btest();
    fibtest();
    acktest();
    return 0;
}
