
#include "./helper.h"

static error *dis_check_mne(x64_state *state, size_t byte_check_array_size, const uint8_t byte_check_array[],
    size_t mne_array_size, const x64_mne mne_array[]) {
    const int32_t byte_idx = 0;
    x64_mem_reset();
    x64_state_init(state, byte_idx, x64_queue_dis_init());
    x64_state_unlock_mem(state);
    memcpy(x64_mem, byte_check_array, byte_check_array_size);
    x64_state_lock_mem(state);
    for (uint8_t inst_idx = 0; inst_idx < mne_array_size; inst_idx++) {
        x64_op op = x64_op_init();
        error *er = x64_dis_next(state, &op);
        if (er)
            return er;
        x64_op_print(&op, stdout, 0, X64_OP_PRINT(DEBUG) | X64_OP_PRINT(NL_END));
        if (op.inst->mne != mne_array[inst_idx])
            return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "x64 invalid dis mne");
    }
    x64_dis_print(byte_idx, stdout, 0, X64_DIS_PRINT(STATE));
    return nullptr;
}

TEST(x64_dis_mov_add_ret) {
    x64_state state;
    const uint8_t byte_check_array[] = { 0x48, 0x89, 0xF8, 0x48, 0x01, 0xF0, 0xC3, 0x61 };
    const x64_mne mne_array[] = { X64_MNE_INST(MOV), X64_MNE_INST(ADD), X64_MNE_INST(RET), X64_MNE(INVALID) };
    error *er = dis_check_mne(&state, DEF_STATIC_ARRAY_SIZE(byte_check_array), byte_check_array,
        DEF_STATIC_ARRAY_SIZE(mne_array), mne_array);
    ASSERT(x64_check_error(&state, er), "x64 invalid dis");
    x64_state_free(&state);
}

TEST(x64_dist_factorial) {
    x64_state state;
    const uint8_t byte_check_array[] = { 0x89, 0xF8, 0x81, 0xF8, 0x01, 0x00, 0x00, 0x00, 0x77, 0x01, 0xC3, 0xFF, 0xCF, 0xF7, 0xE7, 0x81, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x77, 0xF4, 0xC3, 0x61 };
    const x64_mne mne_array[] = { X64_MNE_INST(MOV), X64_MNE_INST(CMP), X64_MNE_INST(JNBE), X64_MNE_INST(RET),
        X64_MNE_INST(DEC), X64_MNE_INST(MUL), X64_MNE_INST(CMP), X64_MNE_INST(JNBE), X64_MNE_INST(RET),
        X64_MNE(INVALID) };
    error *er = dis_check_mne(&state, DEF_STATIC_ARRAY_SIZE(byte_check_array), byte_check_array,
        DEF_STATIC_ARRAY_SIZE(mne_array), mne_array);
    ASSERT(x64_check_error(&state, er), "x64 invalid dis");
    x64_state_free(&state);
}
