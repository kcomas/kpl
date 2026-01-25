
#include "./helper.h"

static bool asm_check_bytes(x64_state *state, const uint8_t check_bytes[]) {
    int32_t check_idx = 0;
    for (int32_t byte_idx = state->byte_start; byte_idx < state->byte_pos; byte_idx++)
        if (x64_mem[byte_idx] != check_bytes[check_idx++])
            return false;
    return true;
}

TEST(x64_asm_mov_add_ret) {
    x64_state state;
    const uint8_t byte_check_array[] = { 0x48, 0x89, 0xF8, 0x48, 0x01, 0xF0, 0xC3, 0x61 };
    x64_mem_reset();
    x64_state_init(&state, x64_mem_get_page_start(), x64_queue_asm_init());
    error *er;
    x64_state_unlock_mem(&state);
    er = x64_asm(&state, X64_MNE_INST(MOV), X64_OP(R64), X64_REG(RAX), X64_OP(R64), X64_REG(RDI), X64_END);
    ASSERT(asm_check_error(&state, er), "x64 invalid mov");
    er = x64_asm(&state, X64_MNE_INST(ADD), X64_OP(R64), X64_REG(RAX), X64_OP(R64), X64_REG(RSI), X64_END);
    ASSERT(asm_check_error(&state, er), "x64 invalid add");
    er = x64_asm(&state, X64_MNE_INST(RET), X64_END);
    ASSERT(asm_check_error(&state, er), "x64 invalid ret");
    er = x64_asm_text_end(&state);
    ASSERT(asm_check_error(&state, er), "x64 invalid text end");
    er = x64_asm_data_end(&state);
    ASSERT(asm_check_error(&state, er), "x64 invalid data end");
    x64_state_lock_mem(&state);
    x64_state_print(&state, stdout, 0, X64_STATE_PRINT(NL_END) | X64_STATE_PRINT(BYTES));
    ASSERT(asm_check_bytes(&state, byte_check_array), "x64 invalid written bytes");
    int64_t a = 5, b = 6;
    int64_t c = ((int64_t(*)(int64_t, int64_t)) &x64_mem[state.byte_start])(a, b);
    printf("%ld + %ld = %ld\n", a, b, c);
    ASSERT(a + b == c, "x64 invalid add");
    x64_state_free(&state);
}
