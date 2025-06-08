
#include "gen_t.h"

static uint8_t *m = NULL;

static __attribute__((constructor)) void gen_c(void) {
    m = x64_mmap(1);
}

static __attribute__((destructor)) void gen_d(void) {
    x64_munmap(1, m);
}

extern const alfr am;

static gen *init(void) {
    gen *g = gen_i(&am, &am, &am, gen_cls_info_tbl, gen_op_tbl(GEN_OP(_END)), gen_mklst());
    gen_b(g);
    return g;
}

static void build(_tests *_t, gen *g, uint8_t *m) {
    E();
    gen_st *st = gen_st_i(&am, &am, gen_op_tbl(20), gen_op_tbl(20), vr_i(16, &am, NULL), vr_i(16, &am, NULL));
    as *a = as_b(as_i(&am, &am, &am, as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
    A(gen_st_p1(g, st) == GEN_STAT(OK), "gen_st_p1");
    gen_st_p(st);
    te *e = NULL;
    gen_stat stat = gen_n(g, st, a, &e);
    if (e) {
        printf("CODE ERROR %p\n", e);
    }
    A(stat == GEN_STAT(OK), "gen_n");
    printf("STATE AFTER\n");
    gen_st_p(st);
    e = NULL;
    as_stat astat = as_n(a, m, &e);
    as_x64_err_p(astat, e);
    A(astat == AS_STAT(OK), "as_n");
    gen_p(g, m);
    printf("DATA\n");
    as_data_p(a, m);
    gen_st_f(st);
    gen_f(g);
    as_f(a);
}

#define BUILD(G, M) build(_t, G, M); \
    E()

T(b) {
    gen *g = gen_i(&am, &am, &am, gen_cls_info_tbl, gen_op_tbl(GEN_OP(_END)), gen_mklst());
    gen_b(g);
    gen_op_p(g->oci, false, 0);
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(ADD), gen_arg(g, X64_TYPE(U6), 0),  gen_arg(g, X64_TYPE(U6), 0), gen_arg(g, X64_TYPE(U6), 1)));
    S(gen_a(g, GEN_OP(LEAVE), gen_arg(g, X64_TYPE(U6), 0), NULL, NULL));
    gen_st *st = gen_st_i(&am, &am, gen_op_tbl(20), gen_op_tbl(20), vr_i(16, &am, NULL), vr_i(16, &am, NULL));
    as *a = as_b(as_i(&am, &am, &am, as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
    A(gen_st_p1(g, st) == GEN_STAT(OK), "gen_st_p1");
    gen_st_p(st);
    te *e = NULL;
    A(gen_n(g, st, a, &e) == GEN_STAT(OK), "gen");
    printf("STATE AFTER\n");
    gen_st_p(st);
    e = NULL;
    A(as_n(a, m, &e) == AS_STAT(OK), "as");
    gen_p(g, m);
    uint64_t arg1 = 3, arg2 = 4;
    uint64_t r = ((uint64_t(*)(uint64_t, uint64_t)) m)(arg1, arg2);
    printf("Add(%lu, %lu): %lu\n", arg1, arg2, r);
    A(arg1 + arg2 == r, "add");
    gen_st_f(st);
    gen_f(g);
    as_f(a);
}

T(call) {
    te *e = NULL;
    as *a = as_b(as_i(&am, &am, &am, as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
    gen *ga = gen_i(&am, &am, &am, gen_cls_info_tbl, gen_op_tbl(GEN_OP(_END)), gen_mklst());
    gen_st *st = gen_st_i(&am, &am, gen_op_tbl(20), gen_op_tbl(20), vr_i(16, &am, NULL), vr_i(16, &am, NULL));
    gen_b(ga);
    S(gen_a(ga, GEN_OP(LBL), gen_lbl(ga, 0), NULL, NULL));
    S(gen_a(ga, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(ga, GEN_OP(ADD), gen_tmp(ga, X64_TYPE(U6), 0),  gen_arg(ga, X64_TYPE(U6), 0), gen_arg(ga, X64_TYPE(U6), 1)));
    S(gen_a(ga, GEN_OP(LEAVE), gen_tmp(ga, X64_TYPE(U6), 0), NULL, NULL));
    A(gen_st_p1(ga, st) == GEN_STAT(OK), "gen_st_p1");
    A(gen_n(ga, st, a, &e) == GEN_STAT(OK), "gen");
    printf("FN0\n");
    gen_p(ga, NULL);
    gen_st_f(st);
    gen *gc = gen_i_gen(ga);
    gen_f(ga);
    st = gen_st_i(&am, &am, gen_op_tbl(20), gen_op_tbl(20), vr_i(16, &am, NULL), vr_i(16, &am, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SUB), gen_arg(gc, X64_TYPE(U6), 0), gen_arg(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(U3), U3(1))));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(U6), 0), gen_call_m(gc, 2, gen_arg(gc, X64_TYPE(U6), 0), gen_arg(gc, X64_TYPE(U6), 1)), gen_lbl(gc, 0)));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(U6), 0), NULL, NULL));
    A(gen_st_p1(gc, st) == GEN_STAT(OK), "gen_st_p1");
    A(gen_n(gc, st, a, &e) == GEN_STAT(OK), "gen");
    printf("FN1\n");
    gen_p(gc, NULL);
    gen_st_f(st);
    gen_f(gc);
    A(as_n(a, m, &e) == AS_STAT(OK), "as");
    as_code_p(a, m);
    int64_t x = 3, y = 5, z = 7;
    te *l1c = as_lbl_g_c(a, 1);
    A(l1c, "l1c");
    A(((int64_t(*)(int64_t, int64_t)) &m[l1c->d[8].u6])(x, y) == z, "call");
    as_f(a);
}

static const char *t_eq_str = "%lu\n";

T(eq) {
    gen *a = gen_b(gen_i(&am, &am, &am, gen_cls_info_tbl, gen_op_tbl(GEN_OP(_END)), gen_mklst()));
    S(gen_a(a, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(a, GEN_OP(ADD), gen_tmp(a, X64_TYPE(U6), 0),  gen_arg(a, X64_TYPE(U6), 0), gen_arg(a, X64_TYPE(U6), 1)));
    S(gen_a(a, GEN_OP(CALLV), gen_call_m(a, 2, gen_data(a, X64_TYPE(M), P(t_eq_str)), gen_arg(a, X64_TYPE(U6), 0)), gen_data(a, X64_TYPE(M), P(printf)), NULL));
    S(gen_a(a, GEN_OP(LEAVE), gen_tmp(a, X64_TYPE(U6), 0), NULL, NULL));
    gen *b = gen_i_gen(a);
    S(gen_a(b, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(b, GEN_OP(ADD), gen_tmp(b, X64_TYPE(U6), 0),  gen_arg(b, X64_TYPE(U6), 0), gen_arg(b, X64_TYPE(U6), 1)));
    S(gen_a(b, GEN_OP(CALLV), gen_call_m(b, 2, gen_data(b, X64_TYPE(M), P(t_eq_str)), gen_arg(b, X64_TYPE(U6), 0)), gen_data(b, X64_TYPE(M), P(printf)), NULL));
    S(gen_a(b, GEN_OP(LEAVE), gen_tmp(b, X64_TYPE(U6), 0), NULL, NULL));
    gen *c = gen_i_gen(b);
    S(gen_a(c, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(c, GEN_OP(ADD), gen_tmp(c, X64_TYPE(I6), 0),  gen_arg(c, X64_TYPE(I6), 0), gen_arg(c, X64_TYPE(I6), 1)));
    S(gen_a(c, GEN_OP(LEAVE), gen_tmp(c, X64_TYPE(I6), 0), NULL, NULL));
    A(gen_code_eq(a, b), "gen_code_eq");
    A(!gen_code_eq(b, c), "!gen_code_eq");
    gen_f(a);
    gen_f(b);
    gen_f(c);
}

T(fib) {
    gen *g = init();
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 0), NULL, NULL));
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    // n == 0
    S(gen_a(g, GEN_OP(NE), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U6), U6(0)), gen_lbl(g, 1)));
    S(gen_a(g, GEN_OP(LEAVE), gen_data(g, X64_TYPE(U6), U6(0)), NULL, NULL));
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 1), NULL, NULL));
    // n > 2
    S(gen_a(g, GEN_OP(GT), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U6), U6(2)), gen_lbl(g, 2)));
    S(gen_a(g, GEN_OP(LEAVE), gen_data(g, X64_TYPE(U6), U6(1)), NULL, NULL));
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 2), NULL, NULL));
    S(gen_a(g, GEN_OP(SUB), gen_tmp(g, X64_TYPE(U6), 0), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U3), U3(1))));
    S(gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(U6), 0), gen_call_m(g, 1, gen_tmp(g, X64_TYPE(U6), 0)), gen_lbl(g, 0)));
    S(gen_a(g, GEN_OP(SUB), gen_tmp(g, X64_TYPE(U6), 1), gen_arg(g, X64_TYPE(U6), 0), gen_data(g, X64_TYPE(U3), U3(2))));
    S(gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(U6), 1), gen_call_m(g, 1, gen_tmp(g, X64_TYPE(U6), 1)), gen_lbl(g, 0)));
    S(gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(U6), 0), gen_tmp(g, X64_TYPE(U6), 0), gen_tmp(g, X64_TYPE(U6), 1)));
    S(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(U6), 0), NULL, NULL));
    BUILD(g, m);
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
    BUILD(g, m);
    uint64_t am = 2, bn = 4 , y = 11;
    //uint64_t am = 3, bn = 10, y = 8189;
    uint64_t r = ((uint64_t(*)(uint64_t, uint64_t)) m)(am, bn);
    printf("Ack(%lu, %lu): %lu\n", am, bn, r);
    A(r == y, "ack");
}

T(addaddaddneg) {
    gen *g = init();
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 0), NULL, NULL));
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(I6), 0), gen_arg(g, X64_TYPE(I6), 1), gen_arg(g, X64_TYPE(I6), 2)));
    S(gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(I6), 1), gen_arg(g, X64_TYPE(I6), 0), gen_tmp(g, X64_TYPE(I6), 0)));
    S(gen_a(g, GEN_OP(NEG), gen_tmp(g, X64_TYPE(I6), 1), gen_tmp(g, X64_TYPE(I6), 1), NULL));
    S(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(I6), 1), NULL, NULL));
    BUILD(g, m);
    int64_t a = 1, b = 2, c = 3, z = -6;
    int64_t r = ((int64_t(*)(int64_t, int64_t, int64_t )) m)(a, b, c);
    printf("-(%ld+%ld+%ld)=%ld\n", a, b, c, r);
    A(r == z, "addaddaddneg");
}

T(printf) {
    gen *g = init();
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(CALLV), gen_call_m(g, 2, gen_data(g, X64_TYPE(M), P("printf: %ld\n")), gen_arg(g, X64_TYPE(I6), 0)), gen_data(g, X64_TYPE(M), P(printf)), NULL));
    S(gen_a(g, GEN_OP(LEAVE), gen_arg(g, X64_TYPE(I6), 0), NULL, NULL));
    BUILD(g, m);
    int64_t a = 789;
    A(a == ((int64_t(*)(int64_t)) m)(a), "printf");
}

//S(gen_a(g, GEN_OP(CALL), gen_call_m(g, 1, gen_arg(g, X64_TYPE(F6), 0)), gen_data(g, X64_TYPE(M), P(fibp)), NULL));
//static void fibp(double n) {
//    printf("n: %lf\n", n);
//}


T(fibxmm) {
    gen *g = init();
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 0), NULL, NULL));
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    // n == 0
    S(gen_a(g, GEN_OP(NE), gen_arg(g, X64_TYPE(F6), 0), gen_data(g, X64_TYPE(F6), F6(0)), gen_lbl(g, 1)));
    S(gen_a(g, GEN_OP(LEAVE), gen_data(g, X64_TYPE(F6), F6(0)), NULL, NULL));
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 1), NULL, NULL));
    // n < 2
    S(gen_a(g, GEN_OP(GT), gen_arg(g, X64_TYPE(F6), 0), gen_data(g, X64_TYPE(F6), F6(2)), gen_lbl(g, 2)));
    S(gen_a(g, GEN_OP(LEAVE), gen_data(g, X64_TYPE(F6), F6(1)), NULL, NULL));
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 2), NULL, NULL));
    S(gen_a(g, GEN_OP(SUB), gen_tmp(g, X64_TYPE(F6), 0), gen_arg(g, X64_TYPE(F6), 0), gen_data(g, X64_TYPE(F6), F6(1))));
    S(gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(F6), 0), gen_call_m(g, 1, gen_tmp(g, X64_TYPE(F6), 0)), gen_lbl(g, 0)));
    S(gen_a(g, GEN_OP(SUB), gen_tmp(g, X64_TYPE(F6), 1), gen_arg(g, X64_TYPE(F6), 0), gen_data(g, X64_TYPE(F6), F6(2))));
    S(gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(F6), 1), gen_call_m(g, 1, gen_tmp(g, X64_TYPE(F6), 1)), gen_lbl(g, 0)));
    S(gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(F6), 0), gen_tmp(g, X64_TYPE(F6), 0), gen_tmp(g, X64_TYPE(F6), 1)));
    S(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(F6), 0), NULL, NULL));
    BUILD(g, m);
    double n = 8.0, y = 21.0;
    //double n = 35.0, y = 9227465.0;
    double r = ((double(*)(double)) m)(n);
    printf("Fibxmm(%lf): %lf == %lf\n", n, r, y);
    A(r == y, "fibxmm");
}

//S(gen_a(g, GEN_OP(CALL), gen_call_m(g, 2, gen_arg(g, X64_TYPE(F6), 0), gen_arg(g, X64_TYPE(F6), 1)), gen_data(g, X64_TYPE(M), P(ackxmmp)), NULL));
//static void ackxmmp(double m, double n) {
//    printf("m: %lf, n: %lf\n", m, n);
//}

T(ackxmm) {
    gen *g = init();
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 0), NULL, NULL));
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    // m == 0
    S(gen_a(g, GEN_OP(NE), gen_arg(g, X64_TYPE(F6), 0), gen_data(g, X64_TYPE(F6), F6(0)), gen_lbl(g, 1)));
    S(gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(F6), 0), gen_arg(g, X64_TYPE(F6), 1), gen_data(g, X64_TYPE(F6), F6(1))));
    S(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(F6), 0), NULL, NULL));
    // m > 0 && n == 0
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 1), NULL, NULL));
    S(gen_a(g, GEN_OP(EQ), gen_arg(g, X64_TYPE(F6), 0), gen_data(g, X64_TYPE(F6), F6(0)), gen_lbl(g, 2)));
    S(gen_a(g, GEN_OP(NE), gen_arg(g, X64_TYPE(F6), 1), gen_data(g, X64_TYPE(F6), F6(0)), gen_lbl(g, 2)));
    S(gen_a(g, GEN_OP(SUB), gen_tmp(g, X64_TYPE(F6), 0), gen_arg(g, X64_TYPE(F6), 0), gen_data(g, X64_TYPE(F6), F6(1))));
    // call ack(m - 1, 1)
    S(gen_a(g, GEN_OP(CALLNPR), gen_tmp(g, X64_TYPE(F6), 0), gen_call_m(g, 2, gen_tmp(g, X64_TYPE(F6), 0), gen_data(g, X64_TYPE(F6), F6(1))), gen_lbl(g, 0)));
    S(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(F6), 0), NULL, NULL));
    // m > 0 && n > 0
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 2), NULL, NULL));
    S(gen_a(g, GEN_OP(EQ), gen_arg(g, X64_TYPE(F6), 0), gen_data(g, X64_TYPE(F6), F6(0)), gen_lbl(g, 3)));
    S(gen_a(g, GEN_OP(EQ), gen_arg(g, X64_TYPE(F6), 1), gen_data(g, X64_TYPE(F6), F6(0)), gen_lbl(g, 3)));
    // call ack(m - 1, ack(m, n - 1))
    S(gen_a(g, GEN_OP(SUB), gen_tmp(g, X64_TYPE(F6), 1), gen_arg(g, X64_TYPE(F6), 1), gen_data(g, X64_TYPE(F6), F6(1))));
    S(gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(F6), 1), gen_call_m(g, 2, gen_arg(g, X64_TYPE(F6), 0), gen_tmp(g, X64_TYPE(F6), 1)), gen_lbl(g, 0)));
    S(gen_a(g, GEN_OP(SUB), gen_tmp(g, X64_TYPE(F6), 2), gen_arg(g, X64_TYPE(F6), 0), gen_data(g, X64_TYPE(F6), F6(1))));
    S(gen_a(g, GEN_OP(CALLNPR), gen_tmp(g, X64_TYPE(F6), 2), gen_call_m(g, 2, gen_tmp(g, X64_TYPE(F6), 2), gen_tmp(g, X64_TYPE(F6), 1)), gen_lbl(g, 0)));
    S(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(F6), 2), NULL, NULL));
    // else
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 3), NULL, NULL));
    S(gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(F6), 3), gen_arg(g, X64_TYPE(F6), 1), gen_data(g, X64_TYPE(F6), F6(1))));
    S(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(F6), 3), NULL, NULL));
    BUILD(g, m);
    double am = 2.0, bn = 4.0 , y = 11.0;
    //double am = 3.0, bn = 10.0, y = 8189.0;
    double r = ((double(*)(double, double)) m)(am, bn);
    printf("Ack(%lf, %lf): %lf == %lf\n", am, bn, r, y);
    A(r == y, "ack");
}

T(printfxmm) {
    gen *g = init();
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(CALLV), gen_call_m(g, 8, gen_data(g, X64_TYPE(M), P("printf(ad:%lf, bd:%lf, cd:%lf, ai:%ld, bi:%ld, ci:%ld, di:%ld)\n")), gen_arg(g, X64_TYPE(F6), 0), gen_arg(g, X64_TYPE(F6), 1), gen_arg(g, X64_TYPE(F6), 2), gen_arg(g, X64_TYPE(I6), 0), gen_arg(g, X64_TYPE(I6), 1), gen_arg(g, X64_TYPE(I6), 2), gen_arg(g, X64_TYPE(I6), 3)), gen_data(g, X64_TYPE(M), P(printf)), NULL));
    S(gen_a(g, GEN_OP(CALLV), gen_call_m(g, 4, gen_data(g, X64_TYPE(M), P("printf(ad:%lf, bd:%lf, cd:%lf)\n")), gen_arg(g, X64_TYPE(F6), 0), gen_arg(g, X64_TYPE(F6), 1), gen_arg(g, X64_TYPE(F6), 2)), gen_data(g, X64_TYPE(M), P(printf)), NULL));
    S(gen_a(g, GEN_OP(CALLV), gen_call_m(g, 5, gen_data(g, X64_TYPE(M), P("printf(ai:%ld, bi:%ld, ci:%ld, di:%ld)\n")), gen_arg(g, X64_TYPE(I6), 0), gen_arg(g, X64_TYPE(I6), 1), gen_arg(g, X64_TYPE(I6), 2), gen_arg(g, X64_TYPE(I6), 3)), gen_data(g, X64_TYPE(M), P(printf)), NULL));
    S(gen_a(g, GEN_OP(LEAVE), gen_arg(g, X64_TYPE(F6), 2), NULL, NULL));
    BUILD(g, m);
    double ad = 1.9, bd = 2.2, cd = 3.3;
    uint64_t ai = 1, bi = 2, ci = 3, di = 4;
    double dd = ((double(*)(double, double, double, uint64_t, uint64_t, uint64_t, uint64_t)) m)(ad, bd, cd, ai, bi, ci, di);
    A(cd == dd, "printxmm");
}
