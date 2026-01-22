
#include "./query.h"

static bool x64_mne_match_by_op(const x64_inst *inst, x64_op_reg op[X64_OP_SIZE]) {
    for (int8_t op_idx = 0; op_idx < X64_OP_SIZE; op_idx++) {
        if (!inst->op[op_idx] && !op[op_idx])
            break;
        if ((inst->op[op_idx] && !op[op_idx]) || (!inst->op[op_idx] && op[op_idx]))
            return false;
        if (!(inst->op[op_idx] & op[op_idx]))
            return false;
        uint32_t reg_mask = x64_op_reg_mask() & inst->op[op_idx];
        if (reg_mask && !(reg_mask & op[op_idx]))
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
        if (inst->flags && op->pfx_flag && !(inst->flags & op->pfx_flag))
            continue;
        if (x64_mne_match_by_op(inst, op->op)) {
            op->inst = inst;
            break;
        }
    }
    return op->inst ? DEF_STATUS(OK) : DEF_STATUS(ERROR);
}
