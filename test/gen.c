
#include "gen_t.h"

T(b) {
    gen *g = gen_i_gen(bg);
    gen_op_p(g->oci, false, 0);
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(ADD), gen_arg(g, X64_TYPE(U6), 0),  gen_arg(g, X64_TYPE(U6), 0), gen_arg(g, X64_TYPE(U6), 1)));
    S(gen_a(g, GEN_OP(LEAVE), gen_arg(g, X64_TYPE(U6), 0), NULL, NULL));
    gen_st *st = gen_st_i(&am, &al_te, gen_op_tbl(10), gen_op_tbl(10), vr_i(16, &al_vr, NULL), vr_i(16, &al_vr, NULL));
    as *a = as_i_as(ba);
    A(gen_st_p1(g, st) == GEN_STAT(OK), "gen_st_p1");
    gen_st_p(st);
    err *e = NULL;
    A(gen_n(g, st, a, &e) == GEN_STAT(OK), "gen");
    printf("STATE AFTER\n");
    gen_st_p(st);
    p = 0;
    e = NULL;
    UNLOCK();
    A(as_n(a, &p, m, &e) == AS_STAT(OK), "as");
    LOCK();
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
    err *e = NULL;
    as *a = as_i_as(ba);
    gen *ga = gen_i_gen(bg);
    gen_st *st = gen_st_i(&am, &al_te, gen_op_tbl(10), gen_op_tbl(10), vr_i(16, &al_vr, NULL), vr_i(16, &al_vr, NULL));
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
    st = gen_st_i(&am, &al_te, gen_op_tbl(10), gen_op_tbl(10), vr_i(16, &al_vr, NULL), vr_i(16, &al_vr, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SUB), gen_arg(gc, X64_TYPE(U6), 0), gen_arg(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(U3), U3(1))));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(U6), 0), gen_call_m(gc, 2, gen_arg(gc, X64_TYPE(U6), 0), gen_arg(gc, X64_TYPE(U6), 1)), gen_lbl(gc, 0)));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(U6), 0), NULL, NULL));
    A(gen_st_p1(gc, st) == GEN_STAT(OK), "gen_st_p1");
    gen_stat stat = gen_n(gc, st, a, &e);
    if (e) err_p(e, true);
    A(stat == GEN_STAT(OK), "gen");
    printf("FN1\n");
    gen_p(gc, NULL);
    gen_st_f(st);
    gen_f(gc);
    p = 0;
    UNLOCK();
    as_stat astat = as_n(a, &p, m, &e);
    LOCK();
    if (e) err_p(e, true);
    A(astat == AS_STAT(OK), "as");
    as_code_p(a, m);
    int64_t x = 3, y = 5, z = 7;
    ssize_t l1c = as_lbl_g_c_i(a, 1);
    A(l1c > 0, "l1c");
    A(((int64_t(*)(int64_t, int64_t)) &m[l1c])(x, y) == z, "call");
    as_f(a);
}

static const char *t_eq_str = "%lu\n";

T(eq) {
    gen *a = gen_b(gen_i(&am, &al_te, &al_vr, &am, gen_cls_info_tbl, gen_op_tbl(GEN_OP(_END)), gen_mklst()));
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
    gen *g = gen_i_gen(bg);
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
    gen *g = gen_i_gen(bg);
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
    gen *g = gen_i_gen(bg);
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
    gen *g = gen_i_gen(bg);
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
    gen *g = gen_i_gen(bg);
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
    gen *g = gen_i_gen(bg);
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
    gen *g = gen_i_gen(bg);
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(CALLV), gen_call_m(g, 8, gen_data(g, X64_TYPE(M), P("printf(ad:%lf, bd:%lf, cd:%lf, ai:%ld, bi:%ld, ci:%ld, di:%ld)\n")), gen_arg(g, X64_TYPE(F6), 0), gen_arg(g, X64_TYPE(F6), 1), gen_arg(g, X64_TYPE(F6), 2), gen_arg(g, X64_TYPE(I6), 0), gen_arg(g, X64_TYPE(I6), 1), gen_arg(g, X64_TYPE(I6), 2), gen_arg(g, X64_TYPE(I6), 3)), gen_data(g, X64_TYPE(M), P(printf)), NULL));
    S(gen_a(g, GEN_OP(CALLV), gen_call_m(g, 4, gen_data(g, X64_TYPE(M), P("printf(ad:%lf, bd:%lf, cd:%lf)\n")), gen_arg(g, X64_TYPE(F6), 0), gen_arg(g, X64_TYPE(F6), 1), gen_arg(g, X64_TYPE(F6), 2)), gen_data(g, X64_TYPE(M), P(printf)), NULL));
    S(gen_a(g, GEN_OP(CALLV), gen_call_m(g, 5, gen_data(g, X64_TYPE(M), P("printf(ai:%ld, bi:%ld, ci:%ld, di:%ld)\n")), gen_arg(g, X64_TYPE(I6), 0), gen_arg(g, X64_TYPE(I6), 1), gen_arg(g, X64_TYPE(I6), 2), gen_arg(g, X64_TYPE(I6), 3)), gen_data(g, X64_TYPE(M), P(printf)), NULL));
    S(gen_a(g, GEN_OP(CALLV), gen_call_m(g, 2, gen_data(g, X64_TYPE(M), P("printf(ad:%lf)\n")), gen_arg(g, X64_TYPE(F6), 0)), gen_data(g, X64_TYPE(M), P(printf)), NULL));
    S(gen_a(g, GEN_OP(CALLV), gen_call_m(g, 3, gen_data(g, X64_TYPE(M), P("%lf, %lf\n")), gen_data(g, X64_TYPE(F6), F6(2.2)), gen_data(g, X64_TYPE(F6), F6(6.6))), gen_data(g, X64_TYPE(M), P(printf)), NULL));
    S(gen_a(g, GEN_OP(LEAVE), gen_arg(g, X64_TYPE(F6), 2), NULL, NULL));
    BUILD(g, m);
    double ad = 1.9, bd = 2.2, cd = 3.3;
    uint64_t ai = 1, bi = 2, ci = 3, di = 4;
    double dd = ((double(*)(double, double, double, uint64_t, uint64_t, uint64_t, uint64_t)) m)(ad, bd, cd, ai, bi, ci, di);
    A(cd == dd, "printxmm");
}

T(fnmulcstdiv) {
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(MUL), gen_tmp(gc, X64_TYPE(F6), 0), gen_arg(gc, X64_TYPE(F6), 0), gen_arg(gc, X64_TYPE(F6), 1)));
    S(gen_a(gc, GEN_OP(CST), gen_tmp(gc, X64_TYPE(F6), 1), gen_arg(gc, X64_TYPE(U6), 0), NULL));
    S(gen_a(gc, GEN_OP(DIV), gen_tmp(gc, X64_TYPE(F6), 2), gen_tmp(gc, X64_TYPE(F6), 0), gen_tmp(gc, X64_TYPE(F6), 1)));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(F6), 2), NULL, NULL));
    BUILD(gc, m);
    double a = 4.4, b = 6.6;
    uint64_t c = 2;
    double d = ((double(*)(double, double, uint64_t)) m)(a, b, c);
    printf("%lf * %lf / %ld = %lf\n", a, b, c, d);
    A(d == 14.52, "eq");
}

T(muldiv) {
    gen *g = gen_i_gen(bg);
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(MUL), gen_tmp(g, X64_TYPE(U6), 0), gen_arg(g, X64_TYPE(U6), 0), gen_arg(g, X64_TYPE(U6), 1)));
    S(gen_a(g, GEN_OP(DIV), gen_tmp(g, X64_TYPE(U6), 1), gen_tmp(g, X64_TYPE(U6), 0), gen_arg(g, X64_TYPE(U6), 2)));
    S(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(U6), 1), NULL, NULL));
    BUILD(g, m);
    uint64_t a = 6, b = 7, c = 2;
    uint64_t r = ((uint64_t(*)(uint64_t, uint64_t, uint64_t)) m)(a, b, c);
    printf("%lu * %lu / %lu = %lu\n", a, b, c, r);
    A(r == 21, "eq");
}

T(set) {
    gen *g = gen_i_gen(bg);
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(SET), gen_stkv(g, X64_TYPE(I6), 0), gen_data(g, X64_TYPE(I6), I6(1234)), NULL));
    S(gen_a(g, GEN_OP(ADD), gen_stkv(g, X64_TYPE(I6), 0), gen_stkv(g, X64_TYPE(I6), 0), gen_arg(g, X64_TYPE(I6), 0)));
    S(gen_a(g, GEN_OP(LEAVE), gen_stkv(g, X64_TYPE(I6), 0), NULL, NULL));
    BUILD(g, m);
    int64_t r = ((int64_t(*)(int64_t)) m)(5);
    A(r == 1239, "seteq");
}

T(facloop) {
    gen *g = gen_i_gen(bg);
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(SET), gen_stkv(g, X64_TYPE(I6), 0), gen_data(g, X64_TYPE(I6), I6(1)), NULL));
    S(gen_a(g, GEN_OP(LT), gen_arg(g, X64_TYPE(I6), 0), gen_data(g, X64_TYPE(I6), U6(2)), gen_lbl(g, 1)));
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 0), NULL, NULL));
    S(gen_a(g, GEN_OP(MUL), gen_stkv(g, X64_TYPE(I6), 0), gen_stkv(g, X64_TYPE(I6), 0), gen_arg(g, X64_TYPE(I6), 0)));
    S(gen_a(g, GEN_OP(SUB), gen_arg(g, X64_TYPE(I6), 0), gen_arg(g, X64_TYPE(I6), 0), gen_data(g, X64_TYPE(I6), I6(1))));
    S(gen_a(g, GEN_OP(GT), gen_arg(g, X64_TYPE(I6), 0), gen_data(g, X64_TYPE(I6), U6(1)), gen_lbl(g, 0)));
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 1), NULL, NULL));
    S(gen_a(g, GEN_OP(LEAVE), gen_stkv(g, X64_TYPE(I6), 0), NULL, NULL));
    BUILD(g, m);
    int64_t a = 5;
    int64_t r = ((int64_t(*)(int64_t)) m)(a);
    printf("fac(%ld): %ld\n", a, r);
    A(r == 120, "eq");
}

T(facxmmloop) {
    gen *g = gen_i_gen(bg);
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(SET), gen_stkv(g, X64_TYPE(F6), 0), gen_data(g, X64_TYPE(F6), F6(1)), NULL));
    S(gen_a(g, GEN_OP(LT), gen_arg(g, X64_TYPE(F6), 0), gen_data(g, X64_TYPE(F6), F6(2)), gen_lbl(g, 0)));
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 0), NULL, NULL));
    S(gen_a(g, GEN_OP(MUL), gen_stkv(g, X64_TYPE(F6), 0), gen_stkv(g, X64_TYPE(F6), 0), gen_arg(g, X64_TYPE(F6), 0)));
    S(gen_a(g, GEN_OP(SUB), gen_arg(g, X64_TYPE(F6), 0), gen_arg(g, X64_TYPE(F6), 0), gen_data(g, X64_TYPE(F6), F6(1))));
    S(gen_a(g, GEN_OP(GT), gen_arg(g, X64_TYPE(F6), 0), gen_data(g, X64_TYPE(F6), F6(1)), gen_lbl(g, 0)));
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 1), NULL, NULL));
    S(gen_a(g, GEN_OP(LEAVE), gen_stkv(g, X64_TYPE(F6), 0), NULL, NULL));
    BUILD(g, m);
    double a = 5;
    double r = ((double(*)(double)) m)(a);
    printf("fac(%lf): %lf\n", a, r);
    A(r == 120.0, "eq");
}

T(updateTE) {
    te *t = te_i(3, &al_te, NULL);
    t->d[0] = I6(1);
    t->d[1] = I6(2);
    t->d[2] = I6(3);
    printf("Before:");
    for (size_t i = 0; i < 3; i++) printf(" %ld", t->d[i].i6);
    putchar('\n');
    gen *g = gen_i_gen(bg);
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 3, gen_arg(g, X64_TYPE(M), 0), gen_data(g, X64_TYPE(U3), U3(sizeof(void*) * 4)), gen_arg(g, X64_TYPE(U6), 1)), gen_arg(g, X64_TYPE(I6), 2), NULL));
    S(gen_a(g, GEN_OP(LEAVE), NULL, NULL, NULL));
    BUILD(g, m);
    ((void(*)(te*, size_t, int64_t)) m)(t, 1, 5);
    A(t->d[1].i6 == 5, "te");
    printf("After:");
    for (size_t i = 0; i < 3; i++) printf(" %ld", t->d[i].i6);
    putchar('\n');
    te_f(t);
}

T(addm) {
    gen *g = gen_i_gen(bg);
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 0), NULL, NULL));
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(ADD), gen_arg(g, X64_TYPE(MU6), 0), gen_arg(g, X64_TYPE(MU6), 0), gen_data(g, X64_TYPE(U6), U6(3))));
    S(gen_a(g, GEN_OP(LEAVE), NULL, NULL, NULL));
    BUILD(g, m);
    uint64_t a = 2;
    ((void(*)(uint64_t*)) m)(&a);
    A(a == 5, "addm");
}

T(callwstck) {
    gen *g = gen_i_gen(bg);
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(CALL), gen_call_w(g, gen_vr(g, 1, gen_arg(g, X64_TYPE(MI6), 0)), NULL), gen_lbl(g, 1), NULL));
    S(gen_a(g, GEN_OP(LEAVE), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 1), NULL, NULL));
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(ADD), gen_stka(g, X64_TYPE(MI6), 0), gen_stka(g, X64_TYPE(MI6), 0), gen_data(g, X64_TYPE(I6), U6(4))));
    S(gen_a(g, GEN_OP(LEAVE), NULL, NULL, NULL));
    BUILD(g, m);
    uint64_t a = 2;
    ((void(*)(uint64_t*)) m)(&a);
    A(a == 6, "stk");
}

T(vrpushprint) {
    gen *g = gen_i_gen(bg);
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(M), 1), gen_arg(g, X64_TYPE(F6), 0), NULL));
    S(gen_a(g, GEN_OP(CALL), gen_call_m(g, 2, gen_arg(g, X64_TYPE(MM), 0), gen_tmp(g, X64_TYPE(M), 1)), gen_data(g, X64_TYPE(M), P(vr_ab)), NULL));
    S(gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(M), 2), gen_arg(g, X64_TYPE(F6), 1), NULL));
    S(gen_a(g, GEN_OP(CALL), gen_call_m(g, 2, gen_arg(g, X64_TYPE(MM), 0), gen_tmp(g, X64_TYPE(M), 2)), gen_data(g, X64_TYPE(M), P(vr_ab)), NULL));
    S(gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(M), 3), gen_arg(g, X64_TYPE(F6), 2), NULL));
    S(gen_a(g, GEN_OP(CALL), gen_call_m(g, 2, gen_arg(g, X64_TYPE(MM), 0), gen_tmp(g, X64_TYPE(M), 3)), gen_data(g, X64_TYPE(M), P(vr_ab)), NULL));
    //for (size_t i = 0; i < v->l; i++) printf("%lf ", v->d[i].f6);
    S(gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(U6), 4), gen_data(g, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(g, GEN_OP(REF), gen_tmp(g, X64_TYPE(M), 5), gen_arg(g, X64_TYPE(MM), 0), NULL));
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 0), NULL, NULL));
    S(gen_a(g, GEN_OP(CALLV), gen_call_m(g, 2, gen_data(g, X64_TYPE(M), P("%lf ")), gen_idx_m(g, X64_TYPE(F6), 3, gen_tmp(g, X64_TYPE(M), 5), gen_data(g, X64_TYPE(U3), U3(sizeof(void*) * 5)), gen_tmp(g, X64_TYPE(U6), 4))), gen_data(g, X64_TYPE(M), P(printf)), NULL));
    S(gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(U6), 4), gen_tmp(g, X64_TYPE(U6), 4), gen_data(g, X64_TYPE(U6), U6(1))));
    S(gen_a(g, GEN_OP(LT), gen_tmp(g, X64_TYPE(U6), 4), gen_idx_m(g, X64_TYPE(U6), 2, gen_tmp(g, X64_TYPE(M), 5), gen_data(g, X64_TYPE(U3), U3(sizeof(void*) * 2))), gen_lbl(g, 0)));
    S(gen_a(g, GEN_OP(CALL), gen_call_m(g, 1, gen_data(g, X64_TYPE(U3), U3('\n'))), gen_data(g, X64_TYPE(M), P(putchar)), NULL));
    S(gen_a(g, GEN_OP(LEAVE), NULL, NULL, NULL));
    BUILD(g, m);
    vr *v = vr_i(5, &al_vr, NULL);
    double a = 1.1, b = 2.2, c = 3.3;
    ((void(*)(vr**, double, double, double)) m)(&v, a, b, c);
    A(v->l == 3, "vr_len");
    A(v->d[0].f6 == a, "vr_d");
    A(v->d[1].f6 == b, "vr_d");
    A(v->d[2].f6 == c, "vr_d");
    vr_f(v);
}

T(argclash) {
    gen *g = gen_i_gen(bg);
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 0), NULL, NULL));
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(I6), 0), gen_call_m(g, 2, gen_arg(g, X64_TYPE(I6), 1), gen_arg(g, X64_TYPE(I6), 0)), gen_lbl(g, 1)));
    S(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(I6), 0), NULL, NULL));
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 1), NULL, NULL));
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(I6), 1), gen_arg(g, X64_TYPE(I6), 0), gen_arg(g, X64_TYPE(I6), 1)));
    S(gen_a(g, GEN_OP(LEAVE), gen_tmp(g, X64_TYPE(I6), 1), NULL, NULL));
    BUILD(g, m);
    int64_t a = 1, b = 2;
    int64_t c = ((int64_t(*)(int64_t,int64_t)) m)(a, b);
    A(c == a + b, "arg clash");
}

T(printfstkf6) {
    gen *g = gen_i_gen(bg);
    S(gen_a(g, GEN_OP(LBL), gen_lbl(g, 0), NULL, NULL));
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(SET), gen_stkv(g, X64_TYPE(F6), 0), gen_data(g, X64_TYPE(F6), F6(3.14)), NULL));
    S(gen_a(g, GEN_OP(SET), gen_arg(g, X64_TYPE(F6), 0), gen_data(g, X64_TYPE(F6), F6(0)), NULL));
    S(gen_a(g, GEN_OP(CALLV), gen_call_m(g, 2, gen_data(g, X64_TYPE(M), P("%lf\n")), gen_stkv(g, X64_TYPE(F6), 0)), gen_data(g, X64_TYPE(M), P(printf)), NULL));
    S(gen_a(g, GEN_OP(LEAVE), gen_stkv(g, X64_TYPE(F6), 0), NULL, NULL));
    BUILD(g, m);
    double x = ((double(*)(void)) m)();
    A(x == 3.14, "print");
}

T(callstk) {
    gen *g = gen_i_gen(bg);
    te *call = gen_call_m(g, 1, gen_data(g, X64_TYPE(M), P("%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu\n")));
    for (size_t i = 0; i < 11; i++) vr_ab((vr**) &call->d[1].p, P(gen_data(g, X64_TYPE(U6), U6(i))));
    te *xcall = gen_call_m(g, 1, gen_data(g, X64_TYPE(M), P("%lf %lf %lf %lf %lf %lf %lf %lf\n")));
    for (size_t i = 0; i < 8; i++) vr_ab((vr**) &xcall->d[1].p, P(gen_data(g, X64_TYPE(F6), F6(i + i / 10.0))));
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(CALLV), call, gen_data(g, X64_TYPE(M), P(printf)), NULL));
    S(gen_a(g, GEN_OP(CALLV), xcall, gen_data(g, X64_TYPE(M), P(printf)), NULL));
    S(gen_a(g, GEN_OP(LEAVE), NULL, NULL, NULL));
    BUILD(g, m);
    ((void(*)(void)) m)();
}

T(incref) {
    te *t = te_i(2, &al_te, NULL);
    gen *g = gen_i_gen(bg);
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(ADD), gen_idx_m(g, X64_TYPE(I6), 2, gen_arg(g, X64_TYPE(M), 0), gen_data(g, X64_TYPE(U3), U3(offsetof(te, r)))), gen_idx_m(g, X64_TYPE(I6), 2, gen_arg(g, X64_TYPE(M), 0), gen_data(g, X64_TYPE(U3), U3(offsetof(te, r)))), gen_data(g, X64_TYPE(I6), I6(1))));
    S(gen_a(g, GEN_OP(LEAVE), NULL, NULL, NULL));
    BUILD(g, m);
    ((void(*)(te*)) m)(t);
    printf("Ref %ld\n", t->r);
    A(t->r == 2, "inv ref");
    te_f(t);
    te_f(t);
}
