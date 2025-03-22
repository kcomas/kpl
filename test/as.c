
#include "as_t.h"


static uint8_t *m = NULL;

__attribute__((constructor)) void as_c(void) {
    m = x64_mmap(1);
}

__attribute__((destructor)) void as_d(void) {
    x64_munmap(1, m);
}

static void as_printf(as *a, const char *fmt) {
    as_a(a, AS_X64(XOR), as_arg_r(a, R(AX)), as_arg_r(a, R(AX)), NULL, NULL);
    as_a(a, AS_X64(MOV), as_arg_r(a, R(DI)), as_arg_qw(a, P(fmt)), NULL, NULL);
    as_a(a, AS_X64(MOV), as_arg_r(a, R(10)), as_arg_qw(a, P(&printf)), NULL, NULL);
    as_a(a, AS_X64(CALL), as_arg_r(a, R(10)), NULL, NULL, NULL);
}

extern const alfr am;

typedef int64_t btestfn(int64_t x);

T(b) {
    as *a = as_b(as_i(&am, &am, &am, as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
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
    A(as_n(a, m) == AS_STAT(OK), "as");
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
    as *a = as_b(as_i(&am, &am, &am, as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
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
    A(as_n(a, m) == AS_STAT(OK), "as");
    as_code_p(a, m);
    A(strcmp(((iftfn*) m)(2), "<") == 0, "<");
    A(strcmp(((iftfn*) m)(7), ">") == 0, ">");
    as_f(a);
}

T(loop) {
    as *a = as_b(as_i(&am, &am, &am, as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
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
    A(as_n(a, m) == AS_STAT(OK), "as");
    as_code_p(a, m);
    int32_t r = ((int32_t(*)(int32_t)) m)(5);
    A(r == 1, "dec");
    as_f(a);
}

T(call) {
    as *a = as_b(as_i(&am, &am, &am, as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
    AS_A2(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_r(a, R(DI)));
    AS_A1(a, AS_X64(JMP), as_arg_l(a, 2));
    as_lbl_a(a, 1);
    AS_A1(a, AS_X64(INC), as_arg_r(a, R(AX)));
    AS_A0(a, AS_X64(RET));
    as_printf(a, "CALL SHOULD NOT SHOW\n");
    as_lbl_a(a, 2);
    AS_A1(a, AS_X64(CALL), as_arg_l(a, 1));
    AS_A0(a, AS_X64(RET));
    A(as_n(a, m) == AS_STAT(OK), "as");
    as_code_p(a, m);
    int32_t v = 5, r = ((int32_t(*)(int32_t)) m)(v);
    printf("call %d, inc: %d\n", v, r);
    A(r == v + 1, "inc");
    as_f(a);
}

T(calle) {
    int64_t r = 32;
    as *a = as_b(as_i(&am, &am, &am, as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
    AS_A1(a, AS_X64(CALL), as_arg_l(a, 0));
    AS_A0(a, AS_X64(RET));
    as_lbl_a(a, 0);
    as_printf(a, "calle\n");
    AS_A2(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_qw(a, I6(r)));
    AS_A0(a, AS_X64(RET));
    A(as_n(a, m) == AS_STAT(OK), "as");
    as_code_p(a, m);
    A(r == ((int64_t(*)()) m)(), "calle");
    as_f(a);
}

T(neg) {
    as *a = as_b(as_i(&am, &am, &am, as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
    AS_A2(a, AS_X64(MOV), as_arg_r(a, R(AX)), as_arg_r(a, R(DI)));
    AS_A1(a, AS_X64(NEG), as_arg_r(a, R(AX)));
    AS_A0(a, AS_X64(RET));
    A(as_n(a, m) == AS_STAT(OK), "as");
    as_code_p(a, m);
    int64_t r = 8;
    A(-r == ((int64_t(*)(int64_t)) m)(r), "neg");
    as_f(a);
}

T(xmmrsp) {
    as *a = as_b(as_i(&am, &am, &am, as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()));
    AS_A2(a, AS_X64(SUB), as_arg_r(a, R(SP)), as_arg_b(a, sizeof(void*) * 2));
    A(AS_A3(a, AS_X64(MOVQ), as_arg_rm(a, R(SP)), as_arg_b(a, 8), as_arg_r(a, XMM(1))) == AS_STAT(OK), "movq_rmbx");
    AS_A2(a, AS_X64(MOVQ), as_arg_rm(a, R(SP)), as_arg_r(a, XMM(2)));
    AS_A2(a, AS_X64(ADD), as_arg_r(a, R(SP)), as_arg_b(a, sizeof(void*) * 2));
    // TODO
    AS_A0(a, AS_X64(RET));
    A(as_n(a, m) == AS_STAT(OK), "as");
    as_code_p(a, m);
    double w = 0, x = 3.14, y = 0.86, z = 4.0;
    A(z == ((double(*)(double, double, double)) m)(w, x, y), "xmmrsp");
    as_f(a);
}
