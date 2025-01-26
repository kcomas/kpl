
#include "gen_t.h"

int main(void) {
    gen *g = gen_i(&malloc, &free, &gen_entry_f, &code_entry_f, &gen_cls_info_tbl, gen_op_tbl(10/*TODO*/), gen_mklst());
    gen_b(g);
    gen_f(g);
    return 0;
}
