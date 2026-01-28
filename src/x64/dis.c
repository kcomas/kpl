
#include "./dis.h"

static error *x64_dis_next_byte(x64_state *state, x64_op *op, uint8_t *byte) {
    *byte = x64_mem[state->byte_pos++];
    op->byte_end++;
    if (!state->byte_pos || state->byte_pos % getpagesize())
        return nullptr;
    return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis hit page boundary");
}

static const x64_inst *x64_dis_load_inital_next_inst(uint8_t next_byte) {
    int16_t inst_index = x64_opcode_table[next_byte][0];
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

static constexpr uint16_t load_byte_flag_mask = X64_FLAG(PREFIX) | X64_FLAG(REX) | X64_FLAG(0F);

static const uint8_t plusr_opcodes[] = { 0x50, 0x58, 0x90, 0xB0, 0xB8, 0xC8 };

static constexpr uint8_t plusr_opcode_size = DEF_STATIC_ARRAY_SIZE(plusr_opcodes);

static error *x64_dis_load_intial_bytes(x64_state *state, x64_op *op) {
    uint8_t next_byte;
    error *er = x64_dis_next_byte(state, op, &next_byte);
    if (er)
        return er;
    const x64_inst *inst = x64_dis_load_inital_next_inst(next_byte);
    if (!inst)
        return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis no inst found");
    while (inst->flags & load_byte_flag_mask) {
        switch (inst->flags & load_byte_flag_mask) {
            case X64_FLAG(PREFIX):
                if (x64_dis_load_inital_next_prefix(inst, op) != DEF_STATUS(OK))
                    return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis prefix all ready set");
                break;
            case X64_FLAG(REX):
                if (op->rex)
                    return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis rex all ready set");
                op->rex = inst->po;
                break;
            case X64_FLAG(0F):
                if (inst->mne != X64_MNE(0F))
                    return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 should not happen");
                if (op->pfx & X64_FLAG(0F))
                    return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis 0F all ready set");
                op->pfx |= X64_FLAG(0F);
                break;
            default:
                return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis should not happen");
        }
        if ((er = x64_dis_next_byte(state, op, &next_byte)))
            return er;
        if (!(inst = x64_dis_load_inital_next_inst(next_byte)))
            return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 dis no inst after prefix rex 0f found");
    }
    op->po = inst->po;
    if (inst->flags & X64_FLAG(PLUSR)) {
        for (int8_t plusr_idx = 0; plusr_idx < plusr_opcode_size; plusr_idx++)
            if (inst->po >= plusr_opcodes[plusr_idx] && inst->po <= plusr_opcodes[plusr_idx] + X64_REG_MAX_ID) {
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
    // TODO label
    return nullptr;
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
    if (op->rm == X64_SIB) {
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
        // TODO label
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
    // TODO label
    return nullptr;
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
    while (!er && (!op.inst || !(op.inst->flags & X64_FLAG(DISASSEMBLER)))) {
        x64_op_reset(&op);
        er = x64_dis_next(&state, &op);
    }
    op = x64_op_init();
    state.byte_pos = state.byte_start;
    while (!er && (!op.inst || !(op.inst->flags & X64_FLAG(DISASSEMBLER)))) {
        x64_op_reset(&op);
        er = x64_dis_next(&state, &op);
        // TODO label
        if (!(op.inst->flags & X64_FLAG(DISASSEMBLER)))
            x64_op_print(&op, file, idnt, X64_OP_PRINT(NL_END));
    }
    if (print_opts & X64_DIS_PRINT(STATE)) {
        memcpy(&state.data_size, x64_mem + state.byte_pos, sizeof(int32_t));
        state.data_pos = state.byte_pos + sizeof(int32_t);
        x64_state_print(&state, file, idnt, X64_STATE_PRINT(NL_END));
    }
    if (er) {
        error_print(er, file, idnt, ERROR_PRINT(NL_END));
        error_free(er);
    }
    x64_state_free(&state);
}
