
#include "as_t.h"

static void as_printf(as *a, const char *fmt) {
    as_a(a, AS_X64(XOR), as_arg_r(a, R(AX)), as_arg_r(a, R(AX)), NULL, NULL);
    as_a(a, AS_X64(MOV), as_arg_r(a, R(DI)), as_arg_qw(a, P(fmt)), NULL, NULL);
    as_a(a, AS_X64(MOV), as_arg_r(a, R(10)), as_arg_qw(a, P(&printf)), NULL, NULL);
    as_a(a, AS_X64(CALL), as_arg_r(a, R(10)), NULL, NULL, NULL);
}

typedef int64_t btestfn(int64_t x);

T(b) {
    as *a = as_i_as(ba);
    as_op_p(a->ops, false, 0);
    as_a(a, AS_X64(NOP), NULL, NULL, NULL, NULL);
    as_a(a, AS_X64(RET), NULL, NULL, NULL, NULL);
    as_lbl_a(a, 1);
    as_a(a, AS_X64(PUSH), as_arg_r(a, R(BP)), NULL, NULL, NULL);
    as_a(a, AS_X64(MOV), as_arg_r(a, R(BP)), as_arg_r(a, R(SP)), NULL, NULL);
    as_a(a, AS_X64(PUSH), as_arg_r(a, R(DI)), NULL, NULL, NULL);
    as_a(a, AS_X64(MOV), as_arg_r(a, R(SI)), as_arg_r(a, R(DI)), NULL, NULL);
    as_printf(a, "Jit Value %ld\n");
    as_a(a, AS_X64(POP), as_arg_r(a, R(AX)), NULL, NULL, NULL);
    as_a(a, AS_X64(POP), as_arg_r(a, R(BP)), NULL, NULL, NULL);
    as_a(a, AS_X64(RET), NULL, NULL, NULL, NULL);
    p = 0;
    err *e = NULL;
    A(as_n(a, &p, m, &e) == AS_STAT(OK), "as");
    as_code_p(a, m);
    te *l1c = as_lbl_g_c(a, 1);
    A(l1c, "lc");
    int64_t v = 1337;
    int64_t r = ((btestfn*) m)(v);
    printf("Call At Start: %ld\n", r);
    A(r != v, "could fail, prob wont");
    r = ((btestfn*) &m[l1c->d[8].u6])(v);
    printf("Call at L(1): %ld\n", r);
    A(r == v, "value not returned");
    as_f(a);
}

typedef const char *iftfn(uint8_t x);

T(ift) {
    as *a = as_i_as(ba);
    as_a(a, AS_X64(PUSH), as_arg_r(a, R(DI)), NULL, NULL, NULL);
    as_a(a, AS_X64(MOV), as_arg_r(a, R(CX)), as_arg_b(a, 5), NULL, NULL);
    as_a(a, AS_X64(CMP), as_arg_r(a, R(DI)), as_arg_r(a, R(CX)), NULL, NULL);
    as_a(a, AS_X64(JNL), as_arg_l(a, 1), NULL, NULL, NULL);
    as_a(a, AS_X64(POP), as_arg_r(a, R(SI)), NULL, NULL, NULL);
    as_printf(a, "%d Less then 5\n");
    as_a(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_qw(a, P("<")), NULL, NULL);
    as_a(a, AS_X64(RET), NULL, NULL, NULL, NULL);
    as_lbl_a(a, 1);
    as_a(a, AS_X64(POP), as_arg_r(a, R(SI)), NULL, NULL, NULL);
    as_printf(a, "%d Greater/Equal To 5\n");
    as_a(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_qw(a, P(">")), NULL, NULL);
    as_a(a, AS_X64(RET), NULL, NULL, NULL, NULL);
    p = 0;
    err *e = NULL;
    A(as_n(a, &p, m, &e) == AS_STAT(OK), "as");
    as_code_p(a, m);
    A(strcmp(((iftfn*) m)(2), "<") == 0, "<");
    A(strcmp(((iftfn*) m)(7), ">") == 0, ">");
    as_f(a);
}

T(loop) {
    as *a = as_i_as(ba);
    AS_A2(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_qw(a, U6(2)));
    AS_A1(a, AS_X64(PUSH), as_arg_r(a, R(AX)));
    AS_A2(a, AS_X64(MOV), as_arg_r(a, R(SI)), as_arg_r(a, R(DI)));
    as_lbl_a(a, 1);
    AS_A1(a, AS_X64(PUSH), as_arg_r(a, R(SI)));
    as_printf(a, "%d > 2: \n");
    AS_A1(a, AS_X64(POP), as_arg_r(a, R(SI)));
    AS_A1(a, AS_X64(DEC), as_arg_r(a, R(SI)));
    AS_A2(a, AS_X64(CMP), as_arg_r(a, R(SI)), as_arg_rm(a, R(SP)));
    AS_A1(a, AS_X64(JNB), as_arg_l(a, 1));
    AS_A1(a, AS_X64(POP), as_arg_r(a, R(AX)));
    AS_A2(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_r(a, R(SI)));
    AS_A0(a, AS_X64(RET));
    p = 0;
    err *e = NULL;
    A(as_n(a, &p, m, &e) == AS_STAT(OK), "as");
    as_code_p(a, m);
    int32_t r = ((int32_t(*)(int32_t)) m)(5);
    A(r == 1, "dec");
    as_f(a);
}

T(call) {
    as *a = as_i_as(ba);
    AS_A2(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_r(a, R(DI)));
    AS_A1(a, AS_X64(JMP), as_arg_l(a, 2));
    as_lbl_a(a, 1);
    AS_A1(a, AS_X64(INC), as_arg_r(a, R(AX)));
    AS_A0(a, AS_X64(RET));
    as_printf(a, "CALL SHOULD NOT SHOW\n");
    as_lbl_a(a, 2);
    AS_A1(a, AS_X64(CALL), as_arg_l(a, 1));
    AS_A0(a, AS_X64(RET));
    p = 0;
    err *e = NULL;
    A(as_n(a, &p, m, &e) == AS_STAT(OK), "as");
    as_code_p(a, m);
    int32_t v = 5, r = ((int32_t(*)(int32_t)) m)(v);
    printf("call %d, inc: %d\n", v, r);
    A(r == v + 1, "inc");
    as_f(a);
}

T(calle) {
    int64_t r = 32;
    as *a = as_i_as(ba);
    AS_A1(a, AS_X64(CALL), as_arg_l(a, 0));
    AS_A0(a, AS_X64(RET));
    as_lbl_a(a, 0);
    as_printf(a, "calle\n");
    AS_A2(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_qw(a, I6(r)));
    AS_A0(a, AS_X64(RET));
    p = 0;
    err *e = NULL;
    A(as_n(a, &p, m, &e) == AS_STAT(OK), "as");
    as_code_p(a, m);
    A(r == ((int64_t(*)()) m)(), "calle");
    as_f(a);
}

T(neg) {
    as *a = as_i_as(ba);
    AS_A2(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_r(a, R(DI)));
    AS_A1(a, AS_X64(NEG), as_arg_r(a, R(AX)));
    AS_A0(a, AS_X64(RET));
    p = 0;
    err *e = NULL;
    A(as_n(a, &p, m, &e) == AS_STAT(OK), "as");
    as_code_p(a, m);
    int64_t r = 8;
    A(-r == ((int64_t(*)(int64_t)) m)(r), "neg");
    as_f(a);
}

T(xmmrsp) {
    as *a = as_i_as(ba);
    AS_A2(a, AS_X64(SUB), as_arg_r(a, R(SP)), as_arg_b(a, sizeof(void*) * 2));
    AS_A3(a, AS_X64(MOVQ), as_arg_rm(a, R(SP)), as_arg_b(a, 8), as_arg_r(a, XMM(1)));
    AS_A2(a, AS_X64(MOVQ), as_arg_rm(a, R(SP)), as_arg_r(a, XMM(2)));
    AS_A3(a, AS_X64(MOVQ), as_arg_r(a, XMM(15)), as_arg_rm(a, R(SP)), as_arg_b(a, 8));
    AS_A2(a, AS_X64(MOVQ), as_arg_r(a, XMM(14)), as_arg_rm(a, R(SP)));
    AS_A2(a, AS_X64(MOVQ), as_arg_r(a, XMM(0)), as_arg_r(a, XMM(15)));
    AS_A2(a, AS_X64(ADDSD), as_arg_r(a, XMM(0)), as_arg_r(a, XMM(14)));
    AS_A2(a, AS_X64(ADD), as_arg_r(a, R(SP)), as_arg_b(a, sizeof(void*) * 2));
    AS_A0(a, AS_X64(RET));
    p = 0;
    err *e = NULL;
    A(as_n(a, &p, m, &e) == AS_STAT(OK), "as");
    as_code_p(a, m);
    double w = 0, x = 3.14, y = 0.86, z = 4.0;
    double r = ((double(*)(double, double, double)) m)(w, x, y);
    printf("%lf + %lf = %lf\n", x, y, r);
    A(z == r, "xmmrsp");
    as_f(a);
}

T(cmprip) {
    as *a = as_i_as(ba);
    AS_A2(a, AS_X64(CMP), as_arg_r(a, R(DI)), as_arg_qw(a, I6(123)));
    AS_A1(a, AS_X64(JE), as_arg_l(a, 1));
    AS_A2(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_qw(a, I6(0)));
    AS_A0(a, AS_X64(RET));
    as_lbl_a(a, 1);
    AS_A2(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_r(a, R(DI)));
    AS_A0(a, AS_X64(RET));
    p = 0;
    err *e = NULL;
    A(as_n(a, &p, m, &e) == AS_STAT(OK), "as");
    as_code_p(a, m);
    A(123 == ((int64_t(*)(int64_t)) m)(123), "eq");
    as_f(a);
}

T(ucomisdrip) {
    as *a = as_i_as(ba);
    AS_A2(a, AS_X64(UCOMISD), as_arg_r(a, XMM(0)), as_arg_qw(a, F6(3.14)));
    AS_A1(a, AS_X64(JE), as_arg_l(a, 1));
    AS_A2(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_qw(a, U6(0)));
    AS_A0(a, AS_X64(RET));
    as_lbl_a(a, 1);
    AS_A2(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_qw(a, U6(1)));
    AS_A0(a, AS_X64(RET));
    p = 0;
    err *e = NULL;
    A(as_n(a, &p, m, &e) == AS_STAT(OK), "as");
    as_code_p(a, m);
    A(1 == ((uint64_t(*)(double)) m)(3.14), "eq");
    as_f(a);
}

T(err) {
    as *a = as_i_as(ba);
    AS_A1(a, AS_X64(JE), as_arg_l(a, 1));
    p = 0;
    err *e = NULL;
    A(as_n(a, &p, m, &e) != AS_STAT(OK), "as");
    err_p(e);
    err_f(e);
    as_f(a);
}

T(mulsddivsd) {
    as *s = as_i_as(ba);
    AS_A2(s, AS_X64(MULSD), as_arg_r(s, XMM(0)), as_arg_r(s, XMM(1)));
    AS_A2(s, AS_X64(CVTSI2SD), as_arg_r(s, XMM(15)), as_arg_r(s, R(DI)));
    AS_A2(s, AS_X64(DIVSD), as_arg_r(s, XMM(0)), as_arg_r(s, XMM(15)));
    AS_A0(s, AS_X64(RET));
    p = 0;
    err *e = NULL;
    A(as_n(s, &p, m, &e) == AS_STAT(OK), "as");
    as_code_p(s, m);
    double a = 4.4, b = 6.6;
    uint64_t c = 2;
    double d = ((double(*)(double, double, uint64_t)) m)(a, b, c);
    printf("%lf * %lf / %ld = %lf\n", a, b, c, d);
    A(d == 14.52, "eq");
    as_f(s);
}

T(arr) {
    as *a = as_i_as(ba);
    AS_A3(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_rm(a, R(DI)), as_arg_b(a, sizeof(int64_t) * 1));
    AS_A0(a, AS_X64(RET));
    p = 0;
    err *e = NULL;
    A(as_n(a, &p, m, &e) == AS_STAT(OK), "as");
    as_code_p(a, m);
    int64_t ar[] = {1, 2, 3};
    A(ar[1] == ((int64_t(*)(int64_t[])) m)(ar), "arr");
    as_f(a);
}

T(sib) {
    te *t = te_i(3, &al_te, NULL);
    t->d[0] = U6(11);
    t->d[1] = U6(22);
    t->d[2] = U6(33);
    as *a = as_i_as(ba);
    AS_A4(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_rm(a, R(DI)), as_arg_rs(a, R(SI), 8), as_arg_b(a, sizeof(void*) * 4));
    AS_A4(a, AS_X64(MOV), as_arg_rm(a, R(DI)), as_arg_rs(a, R(SI), 8), as_arg_b(a, sizeof(void*) * 4), as_arg_r(a, R(DX)));
    AS_A0(a, AS_X64(RET));
    p = 0;
    err *e = NULL;
    A(as_n(a, &p, m, &e) == AS_STAT(OK), "as");
    as_code_p(a, m);
    A(22 == ((uint64_t(*)(te*, size_t, uint64_t)) m)(t, 1, 44), "te g");
    A(t->d[1].u6 == 44, "te s");
    for (size_t i = 0; i < 3; i++) printf("%ld\n", t->d[i].u6);
    as_f(a);
    te_f(t);
}

typedef int64_t retlbladd(int64_t a, int64_t b);

T(retlbl) {
    as *a = as_i_as(ba);
    as_lbl_a(a, 0);
    AS_A1(a, AS_X64(JMP), as_arg_l(a, 2));
    as_lbl_a(a, 1);
    AS_A2(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_r(a, R(DI)));
    AS_A2(a, AS_X64(ADD), as_arg_r(a, R(AX)), as_arg_r(a, R(SI)));
    AS_A0(a, AS_X64(RET));
    as_lbl_a(a, 2);
    AS_A2(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_l(a, 1));
    AS_A0(a, AS_X64(RET));
    p = 0;
    err *e = NULL;
    A(as_n(a, &p, m, &e) == AS_STAT(OK), "as");
    as_code_p(a, m);
    retlbladd *f = ((retlbladd*(*)(void)) m)();
    A(f(1, 2) == 3, "retlbladd");
    as_f(a);
}

T(rcxloop) {
    as *a = as_i_as(ba);
    AS_A2(a, AS_X64(XOR), as_arg_r(a, R(AX)), as_arg_r(a, R(AX)));
    AS_A2(a, AS_X64(MOV), as_arg_r(a, R(CX)), as_arg_b(a, 10));
    as_lbl_a(a, 0);
    AS_A2(a, AS_X64(ADD), as_arg_r(a, R(AX)), as_arg_r(a, R(CX)));
    AS_A1(a, AS_X64(LOOP), as_arg_l(a, 0));
    AS_A0(a, AS_X64(RET));
    p = 0;
    err *e = NULL;
    A(as_n(a, &p, m, &e) == AS_STAT(OK), "as");
    as_code_p(a, m);
    // sum 1 to 10
    A(10 * 11 / 2 == ((uint64_t(*)(void)) m)(), "loop");
    as_f(a);
}
