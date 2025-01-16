
#include "as_t.h"

static void btest(void) {
    as *a = as_i(&malloc, &free, &label_entry_f, &op_entry_f, &code_entry_f, &as_mktbl, as_mktbl(), as_mklst());
    as_lbl_a(a, 1);
    as_code_p(a);
    as_f(a);
}

int main(void) {
    btest();
    return 0;
}
