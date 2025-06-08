
#include "gen_t.h"

static uint8_t *m = NULL;

__attribute__((constructor)) void gen_c(void) {
    m = x64_mmap(1);
}

__attribute__((destructor)) void gen_d(void) {
    x64_munmap(1, m);
}

extern const alfr gm;

#define S(stmt) A((stmt) == GEN_STAT(OK), "stmt")

T(b) {
    gen *g = gen_i(&gm, &gm, gen_entry_f, gen_code_entry_f, gen_cls_info_tbl, gen_op_tbl(GEN_OP(_END)), gen_mklst());
    gen_b(g);
    gen_op_p(g->oci, false, 0);
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(ADD), gen_arg(g, X64_TYPE(U6), 0),  gen_arg(g, X64_TYPE(U6), 0), gen_arg(g, X64_TYPE(U6), 1)));
    S(gen_a(g, GEN_OP(LEAVE), gen_arg(g, X64_TYPE(U6), 0), NULL, NULL));
    gen_st *st = gen_st_i(&gm, &gm, gen_st_atmf, gen_st_latf, gen_op_tbl(20), gen_op_tbl(20), vr_i(16, &gm, NULL), vr_i(16, &gm, NULL));
    as *a = as_b(as_i(&gm, &gm, &gm, as_label_entry_f, as_op_entry_f, as_code_entry_f, as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
    A(gen_st_p1(g, st) == GEN_STAT(OK), "gen_st_p1");
    gen_st_p(st);
    A(gen_n(g, st, a) == GEN_STAT(OK), "gen");
    printf("STATE AFTER\n");
    gen_st_p(st);
    A(as_n(a, m) == AS_STAT(OK), "as");
    gen_p(g, m);
    uint64_t arg1 = 3, arg2 = 4;
    uint64_t r = ((uint64_t(*)(uint64_t, uint64_t)) m)(arg1, arg2);
    printf("Add(%lu, %lu): %lu\n", arg1, arg2, r);
    A(arg1 + arg2 == r, "add");
    gen_st_f(st);
    gen_f(g);
    as_f(a);
}

static gen *init(void) {
    gen *g = gen_i(&gm, &gm, gen_entry_f, gen_code_entry_f, gen_cls_info_tbl, gen_op_tbl(GEN_OP(_END)), gen_mklst());
    gen_b(g);
    return g;
}

static void build(gen *g, uint8_t *m) {
    gen_st *st = gen_st_i(&gm, &gm, gen_st_atmf, gen_st_latf, gen_op_tbl(20), gen_op_tbl(20), vr_i(16, &gm, NULL), vr_i(16, &gm, NULL));
    as *a = as_b(as_i(&gm, &gm, &gm, as_label_entry_f, as_op_entry_f, as_code_entry_f, as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
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

T(fib) {
    gen *g = init();
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 0), NULL, NULL));
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(NE), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U6), U6(0)), gen_lbl(g, 1)));
    S(gen_a(g, GEN_OP(LEAVE), gen_data(g, X64_TYPE(U6), U6(0)), NULL, NULL));
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 1), NULL, NULL));
    S(gen_a(g, GEN_OP(GT), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U6), U6(2)), gen_lbl(g, 2)));
    S(gen_a(g, GEN_OP(LEAVE), gen_data(g, X64_TYPE(U6), U6(1)), NULL, NULL));
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 2), NULL, NULL));
    S(gen_a(g, GEN_OP(SUB), gen_tmp(g, X64_TYPE(U6), 0), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U3), U3(1))));
    S(gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(U6), 0), gen_call_m(g, 1, gen_tmp(g, X64_TYPE(U6), 0)), gen_lbl(g, 0)));
    S(gen_a(g, GEN_OP(SUB), gen_tmp(g, X64_TYPE(U6), 1), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U3), U3(2))));
    S(gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(U6), 1), gen_call_m(g, 1, gen_tmp(g, X64_TYPE(U6), 1)), gen_lbl(g, 0)));
    S(gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(U6), 0), gen_tmp(g, X64_TYPE(U6), 0), gen_tmp(g, X64_TYPE(U6), 1)));
    S(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(U6), 0), NULL, NULL));
    build(g, m);
    uint64_t n = 8, y = 21;
    //uint64_t n = 35, y = 9227465;
    uint64_t r = ((uint64_t(*)(uint64_t)) m)(n);
    printf("Fib(%lu): %lu\n", n, r);
    A(r == y, "fib");
}

T(ack) {
    gen *g = init();
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 0), NULL, NULL));
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    // m == 0
    S(gen_a(g, GEN_OP(NE), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U6), U6(0)), gen_lbl(g, 1)));
    S(gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(U6), 0), gen_arg(g, X64_TYPE(U6), 1), gen_data(g, X64_TYPE(U3), U3(1))));
    S(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(U6), 0), NULL, NULL));
    // m > 0 && n == 0
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 1), NULL, NULL));
    S(gen_a(g, GEN_OP(EQ), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U6), U6(0)), gen_lbl(g, 2)));
    S(gen_a(g, GEN_OP(NE), gen_arg(g, X64_TYPE(U6), 1), gen_data(g, X64_TYPE(U6), U6(0)), gen_lbl(g, 2)));
    S(gen_a(g, GEN_OP(SUB), gen_tmp(g, X64_TYPE(U6), 0), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U3), U3(1))));
    // call ack(m - 1, 1)
    S(gen_a(g, GEN_OP(CALLNPR), gen_tmp(g, X64_TYPE(U6), 0), gen_call_m(g, 2, gen_tmp(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U6), U6(1))), gen_lbl(g, 0)));
    S(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(U6), 0), NULL, NULL));
    // m > 0 && n > 0
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 2), NULL, NULL));
    S(gen_a(g, GEN_OP(EQ), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U6), U6(0)), gen_lbl(g, 3)));
    S(gen_a(g, GEN_OP(EQ), gen_arg(g, X64_TYPE(U6), 1), gen_data(g, X64_TYPE(U6), U6(0)), gen_lbl(g, 3)));
    // call ack(m - 1, ack(m, n - 1))
    S(gen_a(g, GEN_OP(SUB), gen_tmp(g, X64_TYPE(U6), 1), gen_arg(g, X64_TYPE(U6), 1), gen_data(g, X64_TYPE(U3), U3(1))));
    S(gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(U6), 1), gen_call_m(g, 2, gen_arg(g, X64_TYPE(U6), 0), gen_tmp(g, X64_TYPE(U6), 1)), gen_lbl(g, 0)));
    S(gen_a(g, GEN_OP(SUB), gen_tmp(g, X64_TYPE(U6), 2), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U3), U3(1))));
    S(gen_a(g, GEN_OP(CALLNPR), gen_tmp(g, X64_TYPE(U6), 2), gen_call_m(g, 2, gen_tmp(g, X64_TYPE(U6), 2), gen_tmp(g, X64_TYPE(U6), 1)), gen_lbl(g, 0)));
    S(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(U6), 2), NULL, NULL));
    // else
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 3), NULL, NULL));
    S(gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(U6), 3), gen_arg(g, X64_TYPE(U6), 1), gen_data(g, X64_TYPE(U3), U3(1))));
    S(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(U6), 3), NULL, NULL));
    build(g, m);
    uint64_t am = 2, bn = 4 , y = 11;
    //uint64_t am = 3, bn = 10, y = 8189;
    uint64_t r = ((uint64_t(*)(uint64_t, uint64_t)) m)(am, bn);
    printf("Ack(%lu, %lu): %lu\n", am, bn, r);
    A(r == y, "ack");
}
