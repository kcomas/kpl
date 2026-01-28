
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
    error *er;
    x64_mem_reset();
    x64_state_init(&state, x64_mem_get_page_start(), x64_queue_asm_init());
    x64_state_unlock_mem(&state);
    /*
        mov r64rax, r64rdi
        add r64rax, r64rdi
        ret
    */
    er = x64_asm(&state, X64_MNE_INST(MOV), X64_OP(R64), X64_REG(RAX), X64_OP(R64), X64_REG(RDI), X64_END);
    ASSERT(x64_check_error(&state, er), "x64 invalid mov");
    er = x64_asm(&state, X64_MNE_INST(ADD), X64_OP(R64), X64_REG(RAX), X64_OP(R64), X64_REG(RSI), X64_END);
    ASSERT(x64_check_error(&state, er), "x64 invalid add");
    er = x64_asm(&state, X64_MNE_INST(RET), X64_END);
    ASSERT(x64_check_error(&state, er), "x64 invalid ret");
    er = x64_asm_text_end(&state);
    ASSERT(x64_check_error(&state, er), "x64 invalid text end");
    er = x64_asm_data_end(&state);
    ASSERT(x64_check_error(&state, er), "x64 invalid data end");
    x64_state_lock_mem(&state);
    x64_state_print(&state, stdout, 0, X64_STATE_PRINT(NL_END) | X64_STATE_PRINT(BYTES));
    ASSERT(asm_check_bytes(&state, byte_check_array), "x64 invalid written bytes");
    int64_t a = 5, b = 6;
    int64_t c = ((int64_t(*)(int64_t, int64_t)) &x64_mem[state.byte_start])(a, b);
    printf("%ld + %ld = %ld\n", a, b, c);
    ASSERT(a + b == c, "x64 invalid add");
    x64_state_free(&state);
}

TEST(x64_asm_factorial) {
    x64_state state;
    const uint8_t byte_check_array[] = { 0x89, 0xF8, 0x81, 0xF8, 0x01, 0x00, 0x00, 0x00, 0x77, 0x01, 0xC3, 0xFF, 0xCF, 0xF7, 0xE7, 0x81, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x77, 0xF4, 0xC3, 0x61 };
    error *er;
    x64_mem_reset();
    x64_state_init(&state, x64_mem_get_page_start(), x64_queue_asm_init());
    x64_state_unlock_mem(&state);
    /*
        mov r32rax, r32rdi
        cmp r32rax, i32$1
        ja L1
        ret
        L1:
        dec r32rdi
        mul r32rdx, r32rax, r32rdi
        cmp r32rdi, i32$1
        ja L1
        ret
    */
    er = x64_asm(&state, X64_MNE_INST(MOV), X64_OP(R32), X64_REG(RAX), X64_OP(R32), X64_REG(RDI), X64_END);
    ASSERT(x64_check_error(&state, er), "x64 invalid mov");
    er = x64_asm(&state, X64_MNE_INST(CMP), X64_OP(R32), X64_REG(RAX), X64_OP(I32), 1, X64_END);
    ASSERT(x64_check_error(&state, er), "x64 invalid cmp");
    er = x64_asm(&state, X64_MNE_INST(JA), X64_LABEL, X64_OP(REL8), 1, X64_END);
    ASSERT(x64_check_error(&state, er), "x64 invalid ja");
    er = x64_asm(&state, X64_MNE_INST(RET), X64_END);
    ASSERT(x64_check_error(&state, er), "x64 invalid ret");
    er = x64_asm_label(&state, 1, NULL);
    ASSERT(x64_check_error(&state, er), "x64 invalid label");
    er = x64_asm(&state, X64_MNE_INST(DEC), X64_OP(R32), X64_REG(RDI), X64_END);
    ASSERT(x64_check_error(&state, er), "x64 invalid dec");
    er = x64_asm(&state, X64_MNE_INST(MUL), X64_OP(R32), X64_REG(RDX), X64_OP(R32), X64_REG(RAX),
        X64_OP(R32), X64_REG(RDI), X64_END);
    ASSERT(x64_check_error(&state, er), "x64 invalid mul");
    er = x64_asm(&state, X64_MNE_INST(CMP), X64_OP(R32), X64_REG(RDI), X64_OP(I32), 1, X64_END);
    ASSERT(x64_check_error(&state, er), "x64 invalid cmp");
    er = x64_asm(&state, X64_MNE_INST(JA), X64_LABEL, X64_OP(REL8), 1, X64_END);
    ASSERT(x64_check_error(&state, er), "x64 invalid ja");
    er = x64_asm(&state, X64_MNE_INST(RET), X64_END);
    ASSERT(x64_check_error(&state, er), "x64 invalid ret");
    er = x64_asm_text_end(&state);
    ASSERT(x64_check_error(&state, er), "x64 invalid text end");
    er = x64_asm_data_end(&state);
    ASSERT(x64_check_error(&state, er), "x64 invalid data end");
    x64_state_lock_mem(&state);
    x64_state_print(&state, stdout, 0, X64_STATE_PRINT(BYTES));
    ASSERT(asm_check_bytes(&state, byte_check_array), "x64 invalid written bytes");
    int32_t a = 9, b = 362880;
    int32_t c = ((int32_t(*)(int32_t)) &x64_mem[state.byte_start])(a);
    printf("fac(%d) = %d\n", a, b);
    ASSERT(b == c, "x64 invalid factorial");
    x64_state_free(&state);
}
