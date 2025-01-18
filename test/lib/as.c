
#include "as_t.h"

static void btest(void) {
    as *a = as_b();
    as_op_p(a->ops, false, 0);
    as_a(a, AS_INST(NOP), NULL, NULL, NULL, NULL);
    as_lbl_a(a, 1);
    as_a(a, AS_INST(PUSH), as_arg_r(R(BP)), NULL, NULL, NULL);
    as_a(a, AS_INST(MOV), as_arg_r(R(BP)), as_arg_r(R(SP)), NULL, NULL);
    as_a(a, AS_INST(PUSH), as_arg_r(R(DI)), NULL, NULL, NULL);
    as_a(a, AS_INST(XOR), as_arg_r(R(AX)), as_arg_r(R(AX)), NULL, NULL);
    as_a(a, AS_INST(MOV), as_arg_r(R(SI)), as_arg_r(R(DI)), NULL, NULL);
    as_a(a, AS_INST(MOV), as_arg_r(R(DI)), as_arg_qw(P("Jit Value %ld\n")), NULL, NULL);
    as_a(a, AS_INST(MOV), as_arg_r(R(10)), as_arg_qw(P(&printf)), NULL, NULL);
    as_a(a, AS_INST(CALL), as_arg_r(R(10)), NULL, NULL, NULL);
    as_a(a, AS_INST(POP), as_arg_r(R(AX)), NULL, NULL, NULL);
    as_a(a, AS_INST(POP), as_arg_r(R(BP)), NULL, NULL, NULL);
    as_a(a, AS_INST(RET), NULL, NULL, NULL, NULL);
    uint8_t *m = jit_mmap(1);
    if (as_n(a, m) != AS_STAT(OK)) exit(55);
    as_code_p(a, m);
    te *l1;
    if (as_lbl_g_i(a, 1, &l1) == AS_STAT(INV)) exit(66);
    l1 = l1->d[1].p;
    printf("Return: %ld\n", ((int64_t(*)(int64_t)) &m[l1->d[8].u6])(1337));
    jit_munmap(1, m);
    as_f(a);
}

static void ltest(void) {
    as *a = as_b();
    as_f(a);
}

int main(void) {
    btest();
    ltest();
    return 0;
}
