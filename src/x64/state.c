
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
    fprintf(file, "Start: " COLOR(BOLD) "%d" COLOR(RESET) ", Pos: " COLOR(BOLD) "%d" COLOR(RESET),
        state->byte_start, state->byte_pos);
    if (print_opts & X64_STATE_PRINT(BYTES)) {
        fprintf(file, ", Bytes: ");
        for (int32_t byte_idx = state->byte_start; byte_idx < state->byte_pos; byte_idx++)
            fprintf(file, "0x%X ", x64_mem[byte_idx]);
    }
    if (state->queue->used > 0)
        fprintf(file, "\n");
    map_print(state->queue, stdout, 0, MAP_PRINT(_));
    if (print_opts & X64_STATE_PRINT(NL_END))
        fprintf(file, "\n");
}

extern inline x64_op x64_op_init(void);

extern inline void x64_op_reset(x64_op *op);

static void x64_op_print_asm(const x64_op *op, FILE *file, int32_t idnt, x64_op_print_opts print_opts) {
    fprintf(file, "%*s", idnt, "");
    fprintf(file, COLOR(LIGHT_BLUE) "%s " COLOR(RESET), x64_mne_str(op->inst->mne));
    def_mask_print(op->pfx_flag, X64_PFX_FLAG_MAX_BIT, COLOR(LIGHT_GREEN), x64_pfx_flag_str, file);
    for (int8_t op_idx = 0; op_idx < op->op_len; op_idx++) {
        def_mask_print(op->op[op_idx], X64_OP_REG_MAX_BIT, COLOR(LIGHT_RED), x64_op_reg_str, file);
    }
    x64_inst_print_bytes(op->inst, file);
    if (print_opts & X64_OP_PRINT(NL_END))
        fprintf(file, "\n");
}

void x64_op_print(const x64_op *op, FILE *file, int32_t idnt, x64_op_print_opts print_opts) {
    if (!op->inst) {
        fprintf(file, COLOR2(BOLD, RED) "INVALID X64 OP PRINT NO INST\n" COLOR(RESET));
        exit(DEF_EXIT_ERROR);
    }
    if (print_opts & X64_OP_PRINT(ASSEMBLE))
        return x64_op_print_asm(op, file, idnt, print_opts);
}
