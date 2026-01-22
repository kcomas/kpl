
#include "./asm.h"

error *x64_asm_label(x64_state *state, ssize_t label, type_table *fn) {
    if (state->next_label != -1)
        return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "X64 LABEL ALL READY SET");
   state->next_label = label;
   state->next_fn = fn;
   return nullptr;
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
                 op.op[op.op_len++] = next_op | X64_LABEL;
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
    return nullptr;
}

extern inline error *x64_asm(x64_state *state, x64_mne mne, ...);

extern inline error *x64_asm_pfx(x64_state *state, x64_pfx_flag pfx, x64_mne mne, ...);

error *x64_asm_text_end(x64_state *state) {
    return x64_asm(state, X64_MNE_INST(RETF), X64_END);
}
