
#include "./asm.h"

error *x64_asm_label(x64_state *state, ssize_t label, type_table *fn) {
    if (state->next_label != -1)
        return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "X64 LABEL ALL READY SET");
   state->next_label = label;
   state->next_fn = fn;
   return nullptr;
}

static void x64_asm_rex(x64_op *op, x64_rex field) {
    if (!op->rex)
        op->rex = 0x40;
    op->rex |= field;
}

static void x64_finalize_reg(x64_op *op, int8_t *reg, x64_rex field) {
    if (*reg == -1)
        return;
    if (x64_reg_is_upper(*reg))
        x64_asm_rex(op, field);
    *reg = x64_reg_id(*reg);
}

static def_status x64_asm_prep(x64_op *op) {
    int8_t reg_mem_width = -1;
    for (int8_t op_idx = 0; op_idx < op->op_len; op_idx++) {
        int32_t op_mask = op->op[op_idx] & (x64_op_mem_mask() | x64_op_reg_mask());
        if (!op_mask || (op_mask & (X64_OP(XMM) | X64_OP(MM) | X64_OP(M128))))
            continue;
        if (reg_mem_width == -1) {
            reg_mem_width = x64_op_byte_size(op_mask);
            continue;
        }
        if (reg_mem_width != x64_op_byte_size(op_mask))
            return DEF_STATUS(ERROR);
    }
    for (int8_t op_idx = 0; op_idx < op->op_len; op_idx++) {
        int32_t op_mask = op->op[op_idx] & x64_op_mem_mask();
        if (!op_mask)
            continue;
        switch (op_mask) {
            case X64_OP(M8):
                x64_asm_rex(op, X64_REX(REX));
                break;
            case X64_OP(M16):
                op->pfx_flag |= X64_PFX(OPERNAD);
                break;
            case X64_OP(M64):
                x64_asm_rex(op, X64_REX(W));
                break;
            case X64_OP(M32):
            case X64_OP(M128):
                break;
            default:
                return DEF_STATUS(ERROR);
        }
        if (x64_reg_is_upper(op->reg[op_idx]))
            x64_asm_rex(op, X64_REX(B));
        op->rm = op->reg[op_idx];
        if (op->rm != X64_REG(RIP) && (op->dsp_byte_size == 4))
            op->mod = X64_MODSIB(10);
        else if (op->rm == X64_REG(RIP)) {
            op->dsp_byte_size = 4;
            op->mod = X64_MODSIB(00);
        }
        else if ((!op->dsp_byte_size && (op->rm == X64_REG(RBP) || op->rm == X64_REG(R13)))
            || op->dsp_byte_size == 1) {
            op->dsp_byte_size = 1;
            op->mod = X64_MODSIB(01);
        } else
            op->mod = X64_MODSIB(00);
        break;
    }
    for (int8_t op_idx = 0; op_idx < op->op_len; op_idx++) {
        int32_t op_mask = op->op[op_idx] & x64_op_reg_mask();
        if (!op_mask)
            continue;
        if (op->reg[op_idx] == X64_REG(RIP))
            return DEF_STATUS(ERROR);
        switch (op_mask) {
            case X64_OP(R8):
                x64_asm_rex(op, X64_REX(REX));
                break;
            case X64_OP(R16):
                op->pfx_flag |= X64_PFX(OPERNAD);
                break;
            case X64_OP(R64):
                x64_asm_rex(op, X64_REX(W));
                break;
            case X64_OP(R32):
            case X64_OP(XMM):
            case X64_OP(MM):
                break;
            default:
                return DEF_STATUS(ERROR);
        }
        if (op->rm == -1)
            op->rm = op->reg[op_idx];
        else {
            op->r = op->reg[op_idx];
            break;
        }
    }
    if (op->rm != -1 && op->mod == -1)
        op->mod = X64_MODSIB(11);
    if (op->scale != -1 || x64_reg_id(op->rm) == X64_SIB) {
        if (op->scale != -1)
            op->base = op->rm;
        else {
            op->scale = X64_MODSIB(00);
            op->index = op->base = op->rm;
        }
        op->rm = X64_SIB;
    }
    x64_finalize_reg(op, &op->r, X64_REX(R));
    x64_finalize_reg(op, &op->rm, X64_REX(B));
    x64_finalize_reg(op, &op->index, X64_REX(X));
    x64_finalize_reg(op, &op->base, X64_REX(B));
    return DEF_STATUS(OK);
}

static error *x64_asm_write_text_bytes(x64_state *state, const x64_op *op) {
    return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "TODO");
}

static error *x64_asm_va_error(va_list args, const char *msg) {
    va_end(args);
    return ERROR_INIT(0, &def_unused_fn_table, DEF(_), msg);
}

error *_x64_asm(x64_state *state, x64_pfx_flag pfx, x64_mne mne, va_list args) {
    x64_reg reg;
    x64_op op = x64_op_init();
    op.pfx_flag = pfx;
    for (;;) {
        if (op.op_len == X64_OP_SIZE)
            return x64_asm_va_error(args, "x64 too many ops");
        x64_op_reg next_op = va_arg(args, x64_op_reg);
        if (next_op == X64_END)
            break;
        switch (next_op) {
            case X64_OP(R8):
            case X64_OP(R16):
            case X64_OP(R32):
            case X64_OP(R64):
            case X64_OP(M8):
            case X64_OP(M16):
            case X64_OP(M32):
            case X64_OP(M64):
            case X64_OP(M128):
                op.op[op.op_len] = next_op;
                reg = va_arg(args, int32_t);
                if (!x64_reg_is_general(reg))
                    return x64_asm_va_error(args, "x64 invalid general reg");
                op.reg[op.op_len++] = reg;
                break;
            case X64_OP(XMM):
                op.op[op.op_len] = next_op;
                reg = va_arg(args, int32_t);
                if (!x64_reg_is_xmm(reg))
                    return x64_asm_va_error(args, "x64 invalid xmm reg");
                op.reg[op.op_len++] = reg;
                break;
            case X64_OP(MM):
                op.op[op.op_len] = next_op;
                reg = va_arg(args, int32_t);
                if (!x64_reg_is_mm(reg))
                    return x64_asm_va_error(args, "x64 invalid mm reg");
                op.reg[op.op_len++] = reg;
                break;
            case X64_OP(DSP8):
            case X64_OP(DSP32):
                if (op.dsp_byte_size != 0)
                    return x64_asm_va_error(args, "x64 dsp all ready set");
                op.dsp_byte_size = x64_op_byte_size(next_op);
                op.dsp = va_arg(args, int32_t);
                break;
            case X64_OP(I8):
            case X64_OP(I16):
            case X64_OP(I32):
            case X64_OP(I64):
                if (op.imm_byte_size != 0)
                    return x64_asm_va_error(args, "x64 imm all ready set");
                op.op[op.op_len++] = next_op;
                op.imm_byte_size = x64_op_byte_size(next_op);
                op.imm = va_arg(args, def_data);
                break;
            case X64_OP(SCALE1):
            case X64_OP(SCALE2):
            case X64_OP(SCALE4):
            case X64_OP(SCALE8):
                if (op.scale != -1)
                    return x64_asm_va_error(args, "x64 scale all ready set");
                op.scale = x64_op_scale_bits(next_op);
                op.index = va_arg(args, int32_t);
                break;
            case X64_OP(REL8):
            case X64_OP(REL32):
                if (op.rel_byte_size != 0)
                    return x64_asm_va_error(args, "x64 rel all ready set");
                op.op[op.op_len++] = next_op;
                op.rel_byte_size = x64_op_byte_size(next_op);
                op.rel = va_arg(args, int32_t);
                break;
            case X64_LABEL:
                if (op.label != -1)
                    return x64_asm_va_error(args, "x64 label all ready set");
                 next_op = va_arg(args, x64_op_reg);
                 if (!(next_op & x64_op_label_mask()))
                    return x64_asm_va_error(args, "x64 invalid label type");
                 op.label_op = op.op_len;
                 op.op[op.op_len++] = next_op;
                 op.label = va_arg(args, ssize_t);
                 break;
            default:
                return x64_asm_va_error(args, "x64 invalid op");
        }
    }
    va_end(args);
    if (x64_mne_query(mne, &op) != DEF_STATUS(OK))
        return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 no op found");
    x64_op_print(&op, stdout, 0, X64_OP_PRINT(ASSEMBLE) | X64_OP_PRINT(NL_END));
    if (x64_asm_prep(&op) != DEF_STATUS(OK))
        return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 invalid setup");
    x64_op_print(&op, stdout, 0, X64_OP_PRINT(DEBUG) | X64_OP_PRINT(NL_END));
    if (state->next_label) {
        if (x64_queue_add(&state->queue, state->byte_pos, state->next_label, -1, X64_OP_REG(_)) != DEF_STATUS(OK))
            return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 label add failure");
        if (state->next_fn)
            state->next_fn->fn_idx = state->byte_pos;
        state->next_label = -1;
        state->next_fn = nullptr;
    }
    return x64_asm_write_text_bytes(state, &op);
}

extern inline error *x64_asm(x64_state *state, x64_mne mne, ...);

extern inline error *x64_asm_pfx(x64_state *state, x64_pfx_flag pfx, x64_mne mne, ...);

error *x64_asm_text_end(x64_state *state) {
    return x64_asm(state, X64_MNE_INST(RETF), X64_END);
}
