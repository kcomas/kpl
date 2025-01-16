
#include "as_t.h"

static void btest(void) {
    as *a = as_i(&malloc, &free, &label_entry_f, &op_entry_f, &code_entry_f, &as_mktbl, as_mktbl(), as_mklst());
    as_op_a(a, AS_INST(RET), ARG_ID(N), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_ret, NULL);
    as_op_a(a, AS_INST(PUSH), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_push_r, NULL);
    as_op_a(a, AS_INST(POP), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_pop_r, NULL);
    as_op_a(a, AS_INST(MOV), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), &as_mov_rr, NULL);
    as_op_p(a->ops, false, 0);
    as_lbl_a(a, 1);
    as_a(a, AS_INST(PUSH), as_arg_r(R(BP)), NULL, NULL, NULL);
    as_a(a, AS_INST(MOV), as_arg_r(R(BP)), as_arg_r(R(SP)), NULL, NULL);
    as_a(a, AS_INST(MOV), as_arg_r(R(AX)), as_arg_r(R(DI)), NULL, NULL);
    as_a(a, AS_INST(POP), as_arg_r(R(BP)), NULL, NULL, NULL);
    as_a(a, AS_INST(RET), NULL, NULL, NULL, NULL);
    as_code_p(a);
    as_f(a);
}

int main(void) {
    btest();
    return 0;
}
