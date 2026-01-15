
#include "../test.h"

TEST(inst_query) {
    x64_op op = x64_op_init();
    op.pfx_flag |= X64_PFX(LOCK);
    op.op[0] = X64_OP(M64);
    op.op[1] = X64_OP(R64);
    ASSERT(x64_mne_query(X64_MNE_INST(ADD), &op) == DEF_STATUS(OK), "invalid mne query");
    x64_inst_print(op.inst, stdout, 0, X64_INST_PRINT(NL_END));
}
