
#include "./dis.h"

// TODO state and op byte increment

error *x64_dis_next(x64_state *state, x64_op *op) {
    (void) state;
    x64_op_reset(op);
    op->start_byte = op->end_byte = state->byte_pos;
    // TODO possible pf
    // TODO possible rex
    // TODO possible 0F
    // TODO po
    // TODO possible so or possible mod
    // TODO query
    // TODO check fields
    return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "TODO");
}

void x64_dis_print(int32_t byte_idx, FILE *file, int32_t idnt, x64_dis_print_opts print_opts) {
    (void) byte_idx;
    (void) idnt;
    (void) print_opts;
    fprintf(file, COLOR(RED) "TODO\n" COLOR(RESET));
}
