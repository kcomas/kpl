
#include "./helper.h"

static bool asm_check_bytes(x64_state *state, const uint8_t check_bytes[]) {
    int32_t check_idx = 0;
    for (int32_t byte_idx = state->byte_start; byte_idx < state->byte_pos; byte_idx++)
        if (x64_mem[byte_idx] != check_bytes[check_idx++])
            return false;
    return true;
}

#define A(ERROR) ASSERT(x64_check_error(&state, ERROR), "x64 invalid instruction")

TEST(x64_asm_mov_add_ret) {
    x64_state state;
    const uint8_t byte_check_array[] = { 0x48, 0x89, 0xF8, 0x48, 0x01, 0xF0, 0xC3, 0x61 };
    x64_mem_reset();
    x64_state_init(&state, x64_mem_get_page_start(), x64_queue_asm_init());
    x64_state_unlock_mem(&state);
    /*
        mov r64rax, r64rdi
        add r64rax, r64rdi
        ret
    */
    A(x64_asm(&state, X64_MNE_INST(MOV), X64_OP(R64), X64_REG(RAX), X64_OP(R64), X64_REG(RDI), X64_END));
    A(x64_asm(&state, X64_MNE_INST(ADD), X64_OP(R64), X64_REG(RAX), X64_OP(R64), X64_REG(RSI), X64_END));
    A(x64_asm(&state, X64_MNE_INST(RET), X64_END));
    A(x64_asm_text_end(&state));
    A(x64_asm_data_end(&state));
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
    x64_mem_reset();
    x64_state_init(&state, x64_mem_get_page_start(), x64_queue_asm_init());
    x64_state_unlock_mem(&state);
    /*
        mov r32rax, r32rdi
        cmp r32rax, i32$1
        ja L1
        ret
        L1
        dec r32rdi
        mul r32rdx, r32rax, r32rdi
        cmp r32rdi, i32$1
        ja L1
        ret
    */
    A(x64_asm(&state, X64_MNE_INST(MOV), X64_OP(R32), X64_REG(RAX), X64_OP(R32), X64_REG(RDI), X64_END));
    A(x64_asm(&state, X64_MNE_INST(CMP), X64_OP(R32), X64_REG(RAX), X64_OP(I32), 1, X64_END));
    A(x64_asm(&state, X64_MNE_INST(JA), X64_LABEL, X64_OP(REL8), 1, X64_END));
    A(x64_asm(&state, X64_MNE_INST(RET), X64_END));
    A(x64_asm_label(&state, 1, NULL));
    A(x64_asm(&state, X64_MNE_INST(DEC), X64_OP(R32), X64_REG(RDI), X64_END));
    A(x64_asm(&state, X64_MNE_INST(MUL), X64_OP(R32), X64_REG(RDX), X64_OP(R32), X64_REG(RAX),
        X64_OP(R32), X64_REG(RDI), X64_END));
    A(x64_asm(&state, X64_MNE_INST(CMP), X64_OP(R32), X64_REG(RDI), X64_OP(I32), 1, X64_END));
    A(x64_asm(&state, X64_MNE_INST(JA), X64_LABEL, X64_OP(REL8), 1, X64_END));
    A(x64_asm(&state, X64_MNE_INST(RET), X64_END));
    A(x64_asm_text_end(&state));
    A(x64_asm_data_end(&state));
    x64_state_lock_mem(&state);
    x64_state_print(&state, stdout, 0, X64_STATE_PRINT(NL_END) | X64_STATE_PRINT(BYTES));
    ASSERT(asm_check_bytes(&state, byte_check_array), "x64 invalid written bytes");
    int32_t a = 9, b = 362880;
    int32_t c = ((int32_t(*)(int32_t)) &x64_mem[state.byte_start])(a);
    printf("fac(%d) = %d\n", a, c);
    ASSERT(b == c, "x64 invalid factorial");
    x64_state_free(&state);
}

TEST(x64_asm_fib) {
    x64_state state;
    const uint8_t byte_check_array[] = { 0x48, 0x55, 0x48, 0x89, 0xE5, 0x48, 0x83, 0xEC, 0x10, 0x66, 0x0F, 0x2F, 0x05, 0x3E, 0x00, 0x00, 0x00, 0x0F, 0x82, 0x30, 0x00, 0x00, 0x00, 0xF2, 0x0F, 0x11, 0x44, 0x24, 0x08, 0xF2, 0x0F, 0x5C, 0x05, 0x2A, 0x00, 0x00, 0x00, 0xE8, 0xD6, 0xFF, 0xFF, 0xFF, 0xF2, 0x0F, 0x11, 0x04, 0x24, 0xF2, 0x0F, 0x10, 0x44, 0x24, 0x08, 0xF2, 0x0F, 0x5C, 0x05, 0x1A, 0x00, 0x00, 0x00, 0xE8, 0xBE, 0xFF, 0xFF, 0xFF, 0xF2, 0x0F, 0x58, 0x04, 0x24, 0x48, 0x83, 0xC4, 0x10, 0x48, 0x5D, 0xC3, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F };
    x64_mem_reset();
    x64_state_init(&state, x64_mem_get_page_start(), x64_queue_asm_init());
    x64_state_unlock_mem(&state);
    /*
        L0
        push r64rbp
        mov r64rbp, r64rsp
        sub r64rsp, $10
        comisd xmm0, L1
        jb L2
        movsd m64rsp-0x08, xmm0
        subsd xmm0, L1
        call L0
        movsd m64rsp, xmm0
        movsd xmm0, m64rsp-0x08
        subsd xmm0, L3
        call L0
        addsd xmm0, m64rsp
        L2
        add r64rsp, $10
        pop r64rbp
        ret
        L1 $2.0
        L3 $1.0
    */
    A(x64_asm_label(&state, 0, NULL));
    A(x64_asm(&state, X64_MNE_INST(PUSH), X64_OP(R64), X64_REG(RBP), X64_END));
    A(x64_asm(&state, X64_MNE_INST(MOV), X64_OP(R64), X64_REG(RBP), X64_OP(R64), X64_REG(RSP), X64_END));
    A(x64_asm(&state, X64_MNE_INST(SUB), X64_OP(R64), X64_REG(RSP), X64_OP(I8), 0x10, X64_END));
    A(x64_asm(&state, X64_MNE_INST(COMISD), X64_OP(XMM), X64_REG(XMM0), X64_OP(M64), X64_REG(RIP),
        X64_LABEL, X64_OP(DSP32), 1, X64_END));
    A(x64_asm(&state, X64_MNE_INST(JB), X64_LABEL, X64_OP(REL32), 2, X64_END));
    A(x64_asm(&state, X64_MNE_INST(MOVSD), X64_OP(M64), X64_REG(RSP), X64_OP(DSP8), 0x08,
        X64_OP(XMM), X64_REG(XMM0), X64_END));
    A(x64_asm(&state, X64_MNE_INST(SUBSD), X64_OP(XMM), X64_REG(XMM0), X64_OP(M64), X64_REG(RIP),
        X64_LABEL, X64_OP(DSP32), 1, X64_END));
    A(x64_asm(&state, X64_MNE_INST(CALL), X64_LABEL, X64_OP(REL32), 0, X64_END));
    A(x64_asm(&state, X64_MNE_INST(MOVSD), X64_OP(M64), X64_REG(RSP), X64_OP(XMM), X64_REG(XMM0), X64_END));
    A(x64_asm(&state, X64_MNE_INST(MOVSD), X64_OP(XMM), X64_REG(XMM0),
        X64_OP(M64), X64_REG(RSP), X64_OP(DSP8), 0x08, X64_END));
    A(x64_asm(&state, X64_MNE_INST(SUBSD), X64_OP(XMM), X64_REG(XMM0), X64_OP(M64), X64_REG(RIP),
        X64_LABEL, X64_OP(DSP32), 3, X64_END));
    A(x64_asm(&state, X64_MNE_INST(CALL), X64_LABEL, X64_OP(REL32), 0, X64_END));
    A(x64_asm(&state, X64_MNE_INST(ADDSD), X64_OP(XMM), X64_REG(XMM0), X64_OP(M64), X64_REG(RSP), X64_END));
    A(x64_asm_label(&state, 2, NULL));
    A(x64_asm(&state, X64_MNE_INST(ADD), X64_OP(R64), X64_REG(RSP), X64_OP(I8), 0x10, X64_END));
    A(x64_asm(&state, X64_MNE_INST(POP), X64_OP(R64), X64_REG(RBP), X64_END));
    A(x64_asm(&state, X64_MNE_INST(RET), X64_END));
    A(x64_asm_text_end(&state));
    A(x64_asm_data(&state, 1, sizeof(double), DEF_F64(2.0)));
    A(x64_asm_data(&state, 3, sizeof(double), DEF_F64(1.0)));
    A(x64_asm_data_end(&state));
    x64_state_lock_mem(&state);
    x64_state_print(&state, stdout, 0, X64_STATE_PRINT(NL_END) | X64_STATE_PRINT(BYTES));
    ASSERT(asm_check_bytes(&state, byte_check_array), "x64 invalid written bytes");
    double a = 20, b = 6765;
    double c = ((double(*)(double)) &x64_mem[state.byte_start])(a);
    printf("fib(%f) = %f\n", a, c);
    ASSERT(b == c, "x64 invalid factorial");
    x64_state_free(&state);
}
