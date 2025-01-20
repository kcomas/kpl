
#include "as_t.h"

static void as_printf(as *a, const char *fmt) {
    as_a(a, AS_INST(XOR), as_arg_r(R(AX)), as_arg_r(R(AX)), NULL, NULL);
    as_a(a, AS_INST(MOV), as_arg_r(R(DI)), as_arg_qw(P(fmt)), NULL, NULL);
    as_a(a, AS_INST(MOV), as_arg_r(R(10)), as_arg_qw(P(&printf)), NULL, NULL);
    as_a(a, AS_INST(CALL), as_arg_r(R(10)), NULL, NULL, NULL);
}

static void btest(void) {
    as *a = as_b(as_i(&malloc, &free, &label_entry_f, &op_entry_f, &code_entry_f, &as_mktbl, as_mktbl(), as_mklst()));
    as_op_p(a->ops, false, 0);
    printf(">>>> BTEST\n");
    as_a(a, AS_INST(NOP), NULL, NULL, NULL, NULL);
    as_a(a, AS_INST(RET), NULL, NULL, NULL, NULL);
    as_lbl_a(a, 1);
    as_a(a, AS_INST(PUSH), as_arg_r(R(BP)), NULL, NULL, NULL);
    as_a(a, AS_INST(MOV), as_arg_r(R(BP)), as_arg_r(R(SP)), NULL, NULL);
    as_a(a, AS_INST(PUSH), as_arg_r(R(DI)), NULL, NULL, NULL);
    as_a(a, AS_INST(MOV), as_arg_r(R(SI)), as_arg_r(R(DI)), NULL, NULL);
    as_printf(a, "Jit Value %ld\n");
    as_a(a, AS_INST(POP), as_arg_r(R(AX)), NULL, NULL, NULL);
    as_a(a, AS_INST(POP), as_arg_r(R(BP)), NULL, NULL, NULL);
    as_a(a, AS_INST(RET), NULL, NULL, NULL, NULL);
    uint8_t *m = x64_mmap(1);
    if (as_n(a, m) != AS_STAT(OK)) exit(55);
    as_code_p(a, m);
    te *l1c = as_lbl_g_c(a, 1);
    if (!l1c) exit(66);
    printf("Call At Start: %ld\n", ((int64_t(*)(int64_t)) m)(1337));
    printf("Call at L(1): %ld\n", ((int64_t(*)(int64_t)) &m[l1c->d[8].u6])(1337));
    x64_munmap(1, m);
    as_f(a);
    printf("<<<< BTEST\n");
}

static void iftest(void) {
    as *a = as_b(as_i(&malloc, &free, &label_entry_f, &op_entry_f, &code_entry_f, &as_mktbl, as_mktbl(), as_mklst()));
    printf(">>>> IFTEST\n");
    as_a(a, AS_INST(PUSH), as_arg_r(R(DI)), NULL, NULL, NULL);
    as_a(a, AS_INST(MOV), as_arg_r(R(CX)), as_arg_b(5), NULL, NULL);
    as_a(a, AS_INST(CMP), as_arg_r(R(DI)), as_arg_r(R(CX)), NULL, NULL);
    as_a(a, AS_INST(JNL), as_arg_l(1), NULL, NULL, NULL);
    as_a(a, AS_INST(POP), as_arg_r(R(SI)), NULL, NULL, NULL);
    as_printf(a, "%d Less then 5\n");
    as_a(a, AS_INST(RET), NULL, NULL, NULL, NULL);
    as_lbl_a(a, 1);
    as_a(a, AS_INST(POP), as_arg_r(R(SI)), NULL, NULL, NULL);
    as_printf(a, "%d Greater/Equal To 5\n");
    as_a(a, AS_INST(RET), NULL, NULL, NULL, NULL);
    uint8_t *m = x64_mmap(1);
    if (as_n(a, m) != AS_STAT(OK)) exit(55);
    as_code_p(a, m);
    ((void(*)(uint8_t)) m)(2);
    ((void(*)(uint8_t)) m)(7);
    x64_munmap(1, m);
    as_f(a);
    printf("<<<< IFTEST\n");
}

static void looptest(void) {
    as *a = as_b(as_i(&malloc, &free, &label_entry_f, &op_entry_f, &code_entry_f, &as_mktbl, as_mktbl(), as_mklst()));
    printf(">>>> LOOPTEST\n");
    as_a(a, AS_INST(MOV), as_arg_r(R(SI)), as_arg_r(R(DI)), NULL, NULL);
    as_a(a, AS_INST(INC), as_arg_r(R(SI)), NULL, NULL, NULL);
    as_printf(a, "V: %d\n");
    as_a(a, AS_INST(RET), NULL, NULL, NULL, NULL);
    uint8_t *m = x64_mmap(1);
    as_code_p(a, NULL);
    if (as_n(a, m) != AS_STAT(OK)) exit(55);
    ((void(*)(int32_t)) m)(31);
    as_f(a);
    printf("<<<< LOOPTEST\n");
}

int main(void) {
    btest();
    iftest();
    looptest();
    return 0;
}
