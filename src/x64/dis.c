
#include "./dis.h"

static error *x64_dis_next_byte(x64_state *state, x64_op *op, uint8_t *byte) {
    *byte = x64_mem[state->byte_pos++];
    op->byte_end++;
    if (!state->byte_pos || state->byte_pos % getpagesize())
        return nullptr;
    return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis hit page boundary");
}

static const x64_inst *x64_dis_load_inital_next_inst(const x64_op *op, uint8_t next_byte) {
    int16_t index = next_byte;
    if (op->pfx & X64_FLAG(0F))
        index += 0x100;
    int16_t inst_index = x64_opcode_table[index][0];
    if (inst_index == -1)
        return nullptr;
    return &x64_inst_table[inst_index];
}

static def_status x64_dis_load_inital_next_prefix(const x64_inst *inst, x64_op *op) {
    if (!(inst->flags & X64_FLAG(PREFIX)))
        return DEF_STATUS(ERROR);
    x64_pfx_flag pfx = x64_pfx_byte_to_pfx_flag(inst->po);
    if (!pfx || (op->pfx & pfx))
        return DEF_STATUS(ERROR);
    op->pfx |= pfx;
    return DEF_STATUS(OK);
}

static const uint8_t plusr_opcodes[] = { 0x50, 0x58, 0x90, 0xB0, 0xB8, 0xC8 };

static constexpr uint8_t plusr_opcode_size = DEF_STATIC_ARRAY_SIZE(plusr_opcodes);

static error *x64_dis_load_intial_bytes(x64_state *state, x64_op *op) {
    uint8_t next_byte;
    error *er = x64_dis_next_byte(state, op, &next_byte);
    if (er)
        return er;
    const x64_inst *inst = x64_dis_load_inital_next_inst(op, next_byte);
    if (!inst)
        return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis no inst found");
    if (inst->flags & X64_FLAG(PREFIX)) {
        if (x64_dis_load_inital_next_prefix(inst, op) != DEF_STATUS(OK))
            return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis prefix all ready set");
        if ((er = x64_dis_next_byte(state, op, &next_byte)))
            return er;
        if (!(inst = x64_dis_load_inital_next_inst(op, next_byte)))
            return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis no inst after prefix found");
    }
    if (inst->flags & X64_FLAG(REX)) {
        if (op->rex)
            return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis rex all ready set");
        op->rex = inst->po;
        if ((er = x64_dis_next_byte(state, op, &next_byte)))
            return er;
        if (!(inst = x64_dis_load_inital_next_inst(op, next_byte)))
            return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis no inst after rex found");
    }
    if (inst->flags & X64_FLAG(0F)) {
        if (inst->mne != X64_MNE(0F))
            return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 should not happen");
        if (op->pfx & X64_FLAG(0F))
            return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis 0F all ready set");
        op->pfx |= X64_FLAG(0F);
        if ((er = x64_dis_next_byte(state, op, &next_byte)))
            return er;
        if (!(inst = x64_dis_load_inital_next_inst(op, next_byte)))
            return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis no inst after 0f found");
    }
    op->po = inst->po;
    if (inst->flags & X64_FLAG(PLUSR)) {
        for (int8_t plusr_idx = 0; plusr_idx < plusr_opcode_size; plusr_idx++)
            if (inst->po >= plusr_opcodes[plusr_idx] && inst->po <= plusr_opcodes[plusr_idx] + X64_REG_MAX_ID) {
                op->mod = X64_MODSIB(11);
                op->r = 0;
                op->rm = inst->po - plusr_opcodes[plusr_idx];
                break;
            }
        if (op->rm == -1)
            return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis plusr rm not set");
    } else {
        op->next[0] = x64_mem[state->byte_pos];
        if ((state->byte_pos + 1) % getpagesize() != 0)
            op->next[1] = x64_mem[state->byte_pos + 1];
    }
    return nullptr;
}

static error *x64_dis_next_label(x64_state *state, x64_op *op, x64_queue_size byte_size) {
    if (state->next_label == X64_STATE_LABEL_STATUS(NOT_SET))
        return nullptr;
    int32_t label_byte_pos = 0;
    switch (byte_size) {
        case X64_QUEUE_SIZE(8):
            label_byte_pos = (int8_t) x64_mem[state->byte_pos - sizeof(int8_t)];
            break;
        case X64_QUEUE_SIZE(32):
            memcpy(&label_byte_pos, x64_mem + state->byte_pos - sizeof(int32_t), sizeof(int32_t));
            break;
        default:
            return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis invalid byte size for label");
    }
    label_byte_pos += state->byte_pos;
    if (state->next_label == X64_STATE_LABEL_STATUS(PRINT)) {
        const x64_queue_item *queue_item = x64_queue_find(&state->queue, label_byte_pos, state->next_label);
        if (!queue_item)
            return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis unable to print label");
        op->label = queue_item->label;
        return nullptr;
    }
    size_t prev_used = state->queue->used;
    if (x64_queue_add(&state->queue, label_byte_pos, state->next_label, state->byte_pos, byte_size) !=
            DEF_STATUS(OK))
        return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis unable to load label");
    if (state->queue->used > prev_used)
        state->next_label++;
    return nullptr;
}

static error *x64_dis_next_rel(x64_state *state, x64_op *op, uint32_t rel_mask) {
    uint8_t rel_byte_array[sizeof(uint32_t)] = {};
    op->rel_byte_size = x64_op_byte_size(rel_mask);
    if (!op->rel_byte_size)
        return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis invalid byte size for rel");
    for (int8_t rel_byte_len = 0; rel_byte_len < op->rel_byte_size; rel_byte_len++) {
         error *er = x64_dis_next_byte(state, op, &rel_byte_array[rel_byte_len]);
         if (er)
            return er;
    }
    memcpy(&op->rel, rel_byte_array, op->rel_byte_size);
    return x64_dis_next_label(state, op, x64_op_to_queue_size(rel_mask));
}

static void x64_dis_byte_to_bit_parts(uint8_t byte, int8_t *upper2, int8_t *middle3, int8_t *lower3) {
    *upper2 = (byte & (1 << 7 | 1 << 6)) >> 6;
    *middle3 = (byte & (1 << 5 | 1 << 4 | 1 << 3)) >> 3;
    *lower3 = byte & (1 << 2 | 1 << 1 | 1 << 0);
}

static error *x64_dis_next_mod(x64_state *state, x64_op *op) {
    uint8_t modrrmsib;
    error *er = x64_dis_next_byte(state, op, &modrrmsib);
    if (er)
        return er;
    x64_dis_byte_to_bit_parts(modrrmsib, &op->mod, &op->r, &op->rm);
    if (op->mod != X64_MODSIB(11) && op->rm == X64_SIB) {
        if ((er = x64_dis_next_byte(state, op, &modrrmsib)))
            return er;
        x64_dis_byte_to_bit_parts(modrrmsib, &op->scale, &op->index, &op->base);
    }
    if (op->mod == X64_MODSIB(01)) {
        op->dsp_byte_size = sizeof(uint8_t);
        uint8_t dsp_byte;
        if ((er = x64_dis_next_byte(state, op, &dsp_byte)))
            return er;
        op->dsp = dsp_byte;
        return nullptr;
    }
    if (op->mod == X64_MODSIB(11) || (op->mod == X64_MODSIB(00) && op->rm != x64_reg_id(X64_REG(RIP))))
        return nullptr;
    op->dsp_byte_size = sizeof(uint32_t);
    uint8_t dsp_byte_array[sizeof(uint32_t)] = {};
    for (int8_t dsp_byte_len = 0; dsp_byte_len < op->dsp_byte_size; dsp_byte_len++)
        if ((er = x64_dis_next_byte(state, op, &dsp_byte_array[dsp_byte_len])))
            return er;
    memcpy(&op->dsp, dsp_byte_array, op->dsp_byte_size);
    if (op->mod != X64_MODSIB(00) || (op->mod == X64_MODSIB(00) && op->rm != x64_reg_id(X64_REG(RIP))))
        return nullptr;
    return x64_dis_next_label(state, op, x64_op_to_queue_size(X64_OP(DSP32)));
}

static error *x64_dis_next_imm(x64_state *state, x64_op *op, uint32_t imm_mask) {
    uint8_t imm_byte_array[sizeof(uint64_t)] = {};
    switch (imm_mask) {
        case X64_OP(I8):
            op->imm_byte_size = sizeof(uint8_t);
            break;
        case X64_OP(I16) | X64_OP(I32):
            op->imm_byte_size = op->pfx & X64_PFX(OPERAND) ? sizeof(uint16_t) : sizeof(uint32_t);
            break;
        case X64_OP(I64):
            op->imm_byte_size = sizeof(uint64_t);
            break;
        default:
            return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis invalid byte size for imm");
    }
    for (int8_t imm_byte_len = 0; imm_byte_len < op->imm_byte_size; imm_byte_len++) {
        error *er = x64_dis_next_byte(state, op, &imm_byte_array[imm_byte_len]);
        if (er)
            return er;
    }
    memcpy(&op->imm.u64, imm_byte_array, op->imm_byte_size);
    return nullptr;
}

error *x64_dis_next(x64_state *state, x64_op *op) {
    x64_op_reset(op);
    op->byte_start = op->byte_end = state->byte_pos;
    error *er = x64_dis_load_intial_bytes(state, op);
    if (er)
        return er;
    if (x64_opcode_query(op) != DEF_STATUS(OK))
        return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis query no inst found");
    uint8_t byte;
    if (op->inst->so)
        if ((er = x64_dis_next_byte(state, op, &byte)))
            return er;
    if (!op->inst->op[0])
        return nullptr;
    if (op->inst->op[0] & x64_op_rel_mask())
        return x64_dis_next_rel(state, op, op->inst->op[0] & x64_op_rel_mask());
    for (int8_t op_idx = 0; op_idx < X64_OP_SIZE; op_idx++) {
        if (op->mod == -1 && (op->inst->op[op_idx] & (x64_op_reg_mask() | x64_op_mem_mask()))) {
            if ((er = x64_dis_next_mod(state, op)))
                return er;
        }
        if (!op->imm_byte_size && (op->inst->op[op_idx] & x64_op_imm_mask())) {
            if ((er = x64_dis_next_imm(state, op, op->inst->op[op_idx] & x64_op_imm_mask())))
                return er;
        }
    }
    return nullptr;
}

void x64_dis_print(int32_t byte_idx, FILE *file, int32_t idnt, x64_dis_print_opts print_opts) {
    x64_op op = x64_op_init();
    x64_state state;
    error *er = nullptr;
    x64_state_init(&state, byte_idx, x64_queue_dis_init());
    state.next_label = X64_STATE_LABEL_STATUS(LOAD);
    while (!er && (!op.inst || !(op.inst->flags & X64_FLAG(DISASSEMBLER)))) {
        x64_op_reset(&op);
        er = x64_dis_next(&state, &op);
    }
    op = x64_op_init();
    state.byte_pos = state.byte_start;
    state.next_label = X64_STATE_LABEL_STATUS(PRINT);
    for (;;) {
        x64_op_reset(&op);
        const x64_queue_item *queue_item = x64_queue_find(&state.queue, state.byte_pos,
                X64_STATE_LABEL_STATUS(NOT_SET));
        er = x64_dis_next(&state, &op);
        if (er || op.inst->flags & X64_FLAG(DISASSEMBLER))
            break;
        if (queue_item)
            fprintf(file, COLOR(LIGHT_YELLOW) "L%ld\n" COLOR(RESET), queue_item->label);
        x64_op_print(&op, file, idnt, X64_OP_PRINT(NL_END));
    }
    memcpy(&state.data_size, x64_mem + state.byte_pos, sizeof(int32_t));
    fprintf(file, COLOR(LIGHT_YELLOW) "DATA\n" COLOR(RESET) COLOR(BOLD) "%05X" COLOR(RESET)
        COLOR(LIGHT_MAGENTA) "    $%X " COLOR(RESET), state.byte_pos, state.data_size);
    for (const map_bucket *bucket = state.queue->head; bucket; bucket = bucket->next) {
        x64_queue_item *queue_item = bucket->data.ptr;
        if (queue_item->byte_idx > state.byte_pos)
            fprintf(file, COLOR(LIGHT_YELLOW) "L%ld " COLOR(RESET), queue_item->label);
    }
    fprintf(file, "\n");
    if (print_opts & X64_DIS_PRINT(STATE)) {
        state.data_pos = state.byte_pos + sizeof(int32_t);
        x64_state_print(&state, file, idnt, X64_STATE_PRINT(NL_END));
    }
    if (er) {
        error_print(er, file, idnt, ERROR_PRINT(NL_END));
        error_free(er);
    }
    x64_state_free(&state);
}
