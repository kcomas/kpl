
#include "../test.h"

static bool asm_check_error(x64_state *state, error *er) {
    if (!er)
        return true;
    error_print(er, stdout, 0, ERROR_PRINT(NL_END));
    error_free(er);
    x64_state_free(state);
    return false;
}

TEST(mov_add_ret) {
    x64_state state;
    x64_state_init(&state, x64_mem_get_page_start(), x64_queue_asm_init());
    error *er;
    er = x64_asm(&state, X64_MNE_INST(MOV), X64_OP(R64), X64_REG(RAX), X64_OP(R64), X64_REG(RDI), X64_END);
    ASSERT(asm_check_error(&state, er), "x64 invalid mov");
    er = x64_asm(&state, X64_MNE_INST(ADD), X64_OP(R64), X64_REG(RAX), X64_OP(R64), X64_REG(RSI), X64_END);
    ASSERT(asm_check_error(&state, er), "x64 invalid add");
    er = x64_asm(&state, X64_MNE_INST(RET), X64_END);
    ASSERT(asm_check_error(&state, er), "x64 invalid ret");
    er = x64_asm_text_end(&state);
    ASSERT(asm_check_error(&state, er), "x64 invalid text_end");
    x64_state_print(&state, stdout, 0, X64_STATE_PRINT(NL_END) | X64_STATE_PRINT(BYTES));
    // TODO check bytes written
    // 0x48, 0x89, 0xF8, 0x48, 0x01, 0xF0, 0xC3, 0xCB
    x64_state_free(&state);
}
