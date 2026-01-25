
#include "./query.h"

static bool x64_mne_match_by_op(const x64_inst *inst, x64_op_reg op[X64_OP_SIZE], int8_t reg[X64_OP_SIZE]) {
    for (int8_t op_idx = 0; op_idx < X64_OP_SIZE; op_idx++) {
        if (!inst->op[op_idx] && !op[op_idx])
            break;
        if ((inst->op[op_idx] && !op[op_idx]) || (!inst->op[op_idx] && op[op_idx]))
            return false;
        if (!(inst->op[op_idx] & op[op_idx]))
            return false;
        uint32_t reg_mask = x64_op_reg_id_mask() & inst->op[op_idx];
        if (reg_mask && (reg[op_idx] == -1 || (x64_reg_id_mask_id(reg_mask) != x64_reg_id(reg[op_idx]))))
            return false;
    }
    return true;
}

def_status x64_mne_query(x64_mne mne, x64_op *op) {
    if (mne < X64_MNE_INST(ADD) || mne > X64_MNE_INST(PADDD))
        return DEF_STATUS(ERROR);
    const int16_t *inst_idx_array = x64_mne_table[mne];
    while (*inst_idx_array != -1) {
        const x64_inst *inst = &x64_inst_table[*inst_idx_array];
        inst_idx_array++;
        if (inst->flags && op->pfx && !(inst->flags & op->pfx))
            continue;
        if (x64_mne_match_by_op(inst, op->op, op->reg)) {
            op->inst = inst;
            break;
        }
    }
    return op->inst ? DEF_STATUS(OK) : DEF_STATUS(ERROR);
}

static bool x64_opcode_match_by_op(const x64_inst *inst, const x64_op *op) {
    if ((inst->flags & x64_pfx_mask()) && (op->pfx & x64_pfx_mask()) && !(inst->flags & op->pfx))
        return false;
    if (inst->so && inst->so != op->maybe_so)
        return false;
    if (!inst->pf && !(op->pfx & ~(X64_PFX(LOCK) | x64_flag_mask())))
        return true;
    bool found = false;
    for (int8_t pfx_bit_idx = 0; pfx_bit_idx <= X64_PFX_MAX_BIT; pfx_bit_idx++)
        if ((op->pfx & (1 << pfx_bit_idx)) && inst->pf == x64_pfx_byte_by_bit_idx(pfx_bit_idx))
            found = true;
    return found;
}

def_status x64_opcode_query(x64_op *op) {
    if (op->po == -1)
        return DEF_STATUS(ERROR);
    int16_t query_opcode = op->po;
    if (op->pfx & X64_FLAG(0F))
        query_opcode += 0x100;
    if (query_opcode > 0x1FF)
        return DEF_STATUS(ERROR);
    const int16_t *inst_idx_array = x64_opcode_table[query_opcode];
    while (*inst_idx_array != -1) {
        const x64_inst *inst = &x64_inst_table[*inst_idx_array];
        inst_idx_array++;
        if (x64_opcode_match_by_op(inst, op)) {
            op->inst = inst;
            break;
        }
    }
    return op->inst ? DEF_STATUS(OK) : DEF_STATUS(ERROR);
}
