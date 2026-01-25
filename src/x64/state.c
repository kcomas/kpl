
#include "./state.h"

void x64_state_init(x64_state *state, int32_t byte_start, map *queue) {
    state->byte_start = state->byte_pos = byte_start;
    state->data_pos = state->data_size = 0;
    state->next_label = -1;
    state->next_fn = nullptr;
    state->queue = queue;
}

void x64_state_free(x64_state *state) {
    map_free(state->queue);
}

void x64_state_unlock_mem(x64_state *state) {
    x64_mem_unlock(state->byte_start);
}

void x64_state_lock_mem(x64_state *state) {
    x64_mem_lock(state->byte_start);
}

void x64_state_print(const x64_state *state, FILE *file, int32_t idnt, x64_state_print_opts print_opts) {
    fprintf(file, "%*s", idnt, "");
    fprintf(file, "Text Start: " COLOR(BOLD) "%d" COLOR(RESET) ", Pos: " COLOR(BOLD) "%d" COLOR(RESET),
        state->byte_start, state->byte_pos);
    if (print_opts & X64_STATE_PRINT(BYTES)) {
        fprintf(file, ", Bytes: ");
        for (int32_t byte_idx = state->byte_start; byte_idx < state->byte_pos; byte_idx++)
            fprintf(file, "0x%X ", x64_mem[byte_idx]);
    }
    fprintf(file, "\nData Pos: " COLOR(BOLD) "%d" COLOR(RESET) ", Size: " COLOR(BOLD) "%d" COLOR(RESET),
        state->data_pos, state->data_size);
    if (state->queue->used > 0)
        fprintf(file, "\n");
    map_print(state->queue, stdout, 0, MAP_PRINT(_));
    if (print_opts & X64_STATE_PRINT(NL_END))
        fprintf(file, "\n");
}

extern inline x64_op x64_op_init(void);

extern inline void x64_op_reset(x64_op *op);

static void x64_op_print_header(const x64_op *op, FILE *file, int32_t idnt) {
    fprintf(file, "%*s", idnt, "");
    if (op->inst)
        fprintf(file, COLOR(LIGHT_BLUE) "%s " COLOR(RESET), x64_mne_str(op->inst->mne));
    def_mask_print(op->pfx, X64_PFX_FLAG_MAX_BIT, COLOR(LIGHT_GREEN), x64_pfx_flag_str, file);
}

static void x64_op_print_asm(const x64_op *op, FILE *file, int32_t idnt, x64_op_print_opts print_opts) {
    x64_op_print_header(op, file, idnt);
    for (int8_t op_idx = 0; op_idx < op->op_len; op_idx++) {
        def_mask_print(op->op[op_idx], X64_OP_REG_MAX_BIT, COLOR(LIGHT_GREEN), x64_op_reg_str, file);
        if (op->op[op_idx] & x64_op_reg_mask()) {
            fprintf(file, COLOR(LIGHT_RED) "%s " COLOR(RESET), x64_reg_str(op->reg[op_idx]));
            continue;
        }
        if (op->op[op_idx] & x64_op_mem_mask()) {
            fprintf(file, COLOR(LIGHT_RED) "%s" COLOR(RESET), x64_reg_str(op->reg[op_idx]));
            if (op->scale != -1)
                fprintf(file, "+" COLOR(LIGHT_CYAN) "%s*%u" COLOR(RESET),
                    x64_reg_str(op->index), x64_scale_bits_to_size(op->scale));
            if (op->dsp_byte_size)
                fprintf(file, "+" COLOR(GREEN) "0x%X" COLOR(RESET), op->dsp);
            fprintf(file, " ");
            continue;
        }
        if (op->op[op_idx] & x64_op_imm_mask()) {
            fprintf(file, COLOR(LIGHT_MAGENTA) "0x%lX " COLOR(RESET), op->imm.u64);
            continue;
        }
        if (op->op[op_idx] & x64_op_rel_mask())
            fprintf(file, COLOR(BOLD) "0x%X " COLOR(RESET), op->rel);
    }
    if (op->label != -1)
        fprintf(file, COLOR(LIGHT_YELLOW) "L%ld " COLOR(RESET), op->label);
    if (op->inst)
        x64_inst_print_bytes(op->inst, file);
    if (print_opts & X64_OP_PRINT(NL_END))
        fprintf(file, "\n");
}

static void x64_op_debug_field_print(const char *name, FILE *file) {
    fprintf(file, COLOR2(BOLD, CYAN) "%s" COLOR(RESET) ": ", name);
}

static void x64_op_print_debug(const x64_op *op, FILE *file, int32_t idnt, x64_op_print_opts print_opts) {
    if (op->po != -1) {
        x64_op_debug_field_print("po", file);
        fprintf(file, "%X ", op->po);
    }
    if (op->maybe_so != -1) {
        x64_op_debug_field_print("maybe_so", file);
        fprintf(file, "%X? ", op->maybe_so);
    }
    x64_op_print_header(op, file, idnt);
    if (op->rex) {
        x64_op_debug_field_print("rex", file);
        fprintf(file, "0x%X ", op->rex);
    }
    if (op->mod != -1) {
        x64_op_debug_field_print("mod", file);
        fprintf(file, "%d ", op->mod);
        x64_op_debug_field_print("r", file);
        fprintf(file, "%d ", op->r);
        x64_op_debug_field_print("rm", file);
        fprintf(file, "%d ", op->rm);
    }
    if (op->scale != -1) {
        x64_op_debug_field_print("scale", file);
        fprintf(file, "%d ", x64_scale_bits_to_size(op->scale));
        x64_op_debug_field_print("index", file);
        fprintf(file, "%d ", op->index);
        x64_op_debug_field_print("base", file);
        fprintf(file, "%d ", op->base);
    }
    if (op->dsp_byte_size) {
        x64_op_debug_field_print("dsp", file);
        fprintf(file, "0x%X ", op->dsp);
    }
    if (op->rel_byte_size) {
        x64_op_debug_field_print("rel", file);
        fprintf(file, "0x%X ", op->rel);
    }
    if (op->imm_byte_size) {
        x64_op_debug_field_print("imm", file);
        fprintf(file, "0x%lX ", op->imm.u64);
    }
    if (op->label != -1)
        fprintf(file, COLOR(LIGHT_YELLOW) "L%ld " COLOR(RESET), op->label);
    if (op->inst)
        x64_inst_print_bytes(op->inst, file);
    if (print_opts & X64_OP_PRINT(NL_END))
        fprintf(file, "\n");
}

void x64_op_print(const x64_op *op, FILE *file, int32_t idnt, x64_op_print_opts print_opts) {
    if (print_opts & X64_OP_PRINT(ASSEMBLE))
        return x64_op_print_asm(op, file, idnt, print_opts);
    if (print_opts & X64_OP_PRINT(DEBUG))
        return x64_op_print_debug(op, file, idnt, print_opts);
    if (!op->inst) {
        fprintf(file, COLOR2(BOLD, RED) "INVALID X64 OP PRINT NO INST\n" COLOR(RESET));
        exit(DEF_EXIT_ERROR);
    }
}
