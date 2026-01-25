
#include "../test.h"

TEST(inst_mne_query) {
    x64_op op = x64_op_init();
    op.pfx |= X64_PFX(LOCK);
    op.op[0] = X64_OP(M64);
    op.op[1] = X64_OP(R64);
    ASSERT(x64_mne_query(X64_MNE_INST(ADD), &op) == DEF_STATUS(OK), "invalid mne query");
    x64_inst_print(op.inst, stdout, 0, X64_INST_PRINT(NL_END));
    x64_op_reset(&op);
    op.op[0] = X64_OP(R64);
    op.op[1] = X64_OP(R64);
    ASSERT(x64_mne_query(X64_MNE_INST(SUB), &op) == DEF_STATUS(OK), "invalid mne query");
    x64_inst_print(op.inst, stdout, 0, X64_INST_PRINT(NL_END));
    x64_op_reset(&op);
    op.op[0] = op.op[1] = X64_OP(XMM);
    ASSERT(x64_mne_query(X64_MNE_INST(MULSD), &op) == DEF_STATUS(OK), "invalid mne query");
    x64_inst_print(op.inst, stdout, 0, X64_INST_PRINT(NL_END));
}

TEST(inst_opcode_query) {
    x64_op op = x64_op_init();
    op.pfx = x64_byte_to_pfx(0xF3) | X64_FLAG(0F);
    op.po = 0x59;
    op.mod = X64_MODSIB(11);
    op.r = 0;
    op.rm = 1;
    ASSERT(x64_opcode_query(&op) == DEF_STATUS(OK), "invalid opcode query");
    x64_op_print(&op, stdout, 0, X64_OP_PRINT(DEBUG) | X64_OP_PRINT(NL_END));
    ASSERT(op.inst->mne && op.inst->mne == X64_MNE_INST(MULSS), "invalid opcode mne");
    x64_op_reset(&op);
    op.pfx = X64_FLAG(0F);
    op.po = 0xE1;
    op.mod = X64_MODSIB(11);
    op.r = 0;
    op.rm = 1;
    ASSERT(x64_opcode_query(&op) == DEF_STATUS(OK), "invalid opcode query");
    x64_op_print(&op, stdout, 0, X64_OP_PRINT(DEBUG) | X64_OP_PRINT(NL_END));
    ASSERT(op.inst->mne && op.inst->mne == X64_MNE_INST(PSRAW), "invalid opcode mne");
}
