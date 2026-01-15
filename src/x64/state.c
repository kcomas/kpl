
#include "./state.h"

void x64_state_init(x64_state *state, int32_t byte_start, map *queue) {
    state->byte_start = state->byte_pos = byte_start;
    state->next_label = -1;
    state->next_fn = nullptr;
    state->queue = queue;
}

void x64_state_free(x64_state *state) {
    map_free(state->queue);
}

void x64_state_print(const x64_state *state, FILE *file, int32_t idnt, x64_state_print_opts print_opts) {
    fprintf(file, "%*s", idnt, "");
    fprintf(file, "Start: " COLOR(BOLD) "%d" COLOR(RESET) ", Pos" COLOR(BOLD) "%d" COLOR(RESET),
        state->byte_start, state->byte_pos);
    map_print(state->queue, stdout, 0, MAP_PRINT(_));
    if (print_opts & X64_STATE_PRINT(NL_END))
        fprintf(file, "\n");
}

extern inline x64_op x64_op_init(void);

extern inline void x64_op_reset(x64_op *op);
