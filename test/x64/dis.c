
#include "./helper.h"

TEST(x64_dis_mov_add_ret) {
    x64_state state;
    const uint8_t byte_check_array[] = { 0x48, 0x89, 0xF8, 0x48, 0x01, 0xF0, 0xC3, 0x61 };
    const x64_mne mne_array[] = { X64_MNE_INST(MOV), X64_MNE_INST(ADD), X64_MNE_INST(RET), X64_MNE(INVALID) };
    const int32_t byte_idx = 0;
    x64_mem_reset();
    x64_state_init(&state, byte_idx, x64_queue_dis_init());
    x64_state_unlock_mem(&state);
    memcpy(x64_mem, byte_check_array, sizeof(byte_check_array) / sizeof(byte_check_array[0]));
    x64_state_lock_mem(&state);
    for (uint8_t inst_idx = 0; inst_idx < sizeof(mne_array) / sizeof(mne_array[0]); inst_idx++) {
        x64_op op = x64_op_init();
        error *er = x64_dis_next(&state, &op);
        ASSERT(asm_check_error(&state, er), "x64 invalid dis next");
        x64_op_print(&op, stdout, 0, X64_OP_PRINT(DEBUG) | X64_OP_PRINT(NL_END));
        ASSERT(op.inst->mne == mne_array[inst_idx], "x64 invalid dis mne");
    }
    x64_dis_print(byte_idx, stdout, 0, X64_DIS_PRINT(_));
    x64_state_free(&state);
}
