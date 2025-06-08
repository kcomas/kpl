
#pragma once

#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include "def.h"

extern size_t p;

extern uint8_t *m;

#define X64_STAT(N) JIT_STAT_##N

typedef enum {
    X64_STAT(OK),
    X64_STAT(INV_SIZE),
    X64_STAT(INV_REG)
} x64_stat;

#define R(N) R_##N

#define XMM(N) X_##N

typedef enum {
    R(AX),
    R(CX),
    R(DX),
    R(BX),
    R(SP),
    R(BP),
    R(SI),
    R(DI),
    R(8),
    R(9),
    R(10),
    R(11),
    R(12),
    R(13),
    R(14),
    R(15),
    XMM(0),
    XMM(1),
    XMM(2),
    XMM(3),
    XMM(4),
    XMM(5),
    XMM(6),
    XMM(7),
    XMM(8),
    XMM(9),
    XMM(10),
    XMM(11),
    XMM(12),
    XMM(13),
    XMM(14),
    XMM(15)
} reg;

// 0-7 gen lower, 8-15 // gen upper
// 16-23 xmm lower 24-32 xmm upper

#define REX(N) REX_##N

typedef enum {
    REX(B) = 0x41,
    REX(X) = 0x42,
    REX(R) = 0x44,
    REX(W) = 0x48
} rex;

#define MOD(N) MOD_##N

typedef enum {
    MOD(00) = 0x00,
    MOD(01) = 0x40,
    MOD(10) = 0x80,
    MOD(11) = 0xC0
} mod;

typedef enum {
    S1 = 0x00,
    S2 = 0x40,
    S4 = 0x80,
    S8 = 0xC0
} scale;

// map page aligned
uint8_t *x64_mmap(size_t size);

// unmap page aligned
void x64_munmap(size_t size, uint8_t *m);

x64_stat x64_a(size_t *p, uint8_t *m, uint8_t b);

x64_stat x64_b(size_t *p, uint8_t *m, size_t len, ...);

x64_stat x64_c(size_t *p, uint8_t *m, size_t len, uint8_t b[]);

x64_stat x64_d(size_t *p, uint8_t *m, un u);

x64_stat x64_e(size_t *p, uint8_t *m, size_t size, un v);

#define X64_RS() asm("push %rbx\n\t" \
        "push %rcx\n\t" \
        "push %rdx\n\t" \
        "push %r12\n\t" \
        "push %r13\n\t" \
        "push %r14\n\t" \
        "push %r15\n\t")

#define X64_RR() asm("pop %r15\n\t" \
        "pop %r14\n\t" \
        "pop %r13\n\t" \
        "pop %r12\n\t" \
        "pop %rdx\n\t" \
        "pop %rcx\n\t" \
        "pop %rbx\n\t")

// nop
x64_stat x64_nop(size_t *p, uint8_t *m);

// ret
x64_stat x64_ret(size_t *p, uint8_t *m);

// leave
x64_stat x64_leave(size_t *p, uint8_t *m);

// push rax
x64_stat x64_push_r(size_t *p, uint8_t *m, reg r);

// pop rax
x64_stat x64_pop_r(size_t *p, uint8_t *m, reg r);

// call rax
x64_stat x64_call_r(size_t *p, uint8_t *m, reg r);

// call dword
x64_stat x64_call_d(size_t *p, uint8_t *m, uint32_t dw);

// mov rax, 64bit value
x64_stat x64_mov_rq(size_t *p, uint8_t *m, reg r, un u);

// mov rax, rdi
x64_stat x64_mov_rr(size_t *p, uint8_t *m, reg d, reg s);

// mov qword ptr[rdi], rax
x64_stat x64_mov_rmr(size_t *p, uint8_t *m, reg d, reg s);

// mov qword ptr[rdi+dsp8], rax
x64_stat x64_mov_rmbr(size_t *p, uint8_t *m, reg d, uint8_t dsp, reg s);

// mov qword ptr[rdi+rsi*x], rax
x64_stat x64_mov_rmor(size_t *p, uint8_t *m, reg d, reg o, scale x, reg s);

// mov qword ptr[rdi+rsi*x+dsp8], rax
x64_stat x64_mov_rmobr(size_t *p, uint8_t *m, reg d, reg o, scale x, uint8_t dsp, reg s);

// mov rax, qword ptr [rdi]
x64_stat x64_mov_rrm(size_t *p, uint8_t *m, reg d, reg s);

// mov rax, qword ptr[rdi+dsp8]
x64_stat x64_mov_rrmb(size_t *p, uint8_t *m, reg d, reg s, uint8_t dsp);

// mov rax qword ptr[rdi+rsi*x]
x64_stat x64_mov_rrmo(size_t *p, uint8_t *m, reg d, reg s, reg o, scale x);

// mov rax qword ptr[rdi+rsi*x+dsp8]
x64_stat x64_mov_rrmob(size_t *p, uint8_t *m, reg d, reg s, reg o, scale x, uint8_t dsp);

// mov qword ptr[rax+dsp8] imm32
x64_stat x64_mov_rmbd(size_t *p, uint8_t *m, reg r, uint8_t dsp, uint32_t d);

// movq rax, xmm0
x64_stat x64_movq_rx(size_t *p, uint8_t *m, reg d, reg s);

// movq qword ptr[rax], xmm0
x64_stat x64_movq_rmx(size_t *p, uint8_t *m, reg d, reg s);

// movq qword ptr[rax+dsp8], xmm0
x64_stat x64_movq_rmbx(size_t *p, uint8_t *m, reg d, uint8_t dsp, reg s);

// movq xmm0, rax
x64_stat x64_movq_xr(size_t *p, uint8_t *m, reg d, reg s);

// movq xmm0 qword ptr[rax]
x64_stat x64_movq_xrm(size_t *p, uint8_t *m, reg d, reg s);

// movq xmm0 qword ptr[rax+dsp8]
x64_stat x64_movq_xrmb(size_t *p, uint8_t *m, reg d, reg s, uint8_t dsp);

// movq xmm0, xmm1
x64_stat x64_movq_xx(size_t *p, uint8_t *m, reg d, reg s);

// movq xmm0, qword ptr[rip+dsp32]
x64_stat x64_movq_xi(size_t *p, uint8_t *m, reg d, uint32_t dsp);

// movsd xmm0, xmm1
x64_stat x64_movsd_xx(size_t *p, uint8_t *m, reg d, reg s);

// movsd xmm0, qword ptr[rsp]
x64_stat x64_movsd_xrm(size_t *p, uint8_t *m, reg d, reg s);

// movsd xmm0, qword ptr[rsp+dsp8]
x64_stat x64_movsd_xrmb(size_t *p, uint8_t *m, reg d, reg s, uint8_t dsp);

// movsd xmm0, qword ptr[rip+disp32]
x64_stat x64_movsd_xi(size_t *p, uint8_t *m, reg d, uint32_t dsp);

// movsd qword ptr[rsp], xmm0
x64_stat x64_movsd_rmx(size_t *p, uint8_t *m, reg d, reg s);

// movsd qword ptr[rsp+dsp8], xmm0
x64_stat x64_movsd_rmbx(size_t *p, uint8_t *m, reg d, uint8_t dsp, reg s);

// mov xxm0, qword ptr[rsp+rsi*x]
x64_stat x64_movsd_xrmo(size_t *p, uint8_t *m, reg d, reg s, reg o, scale x);

// move xmm0, qword ptr[rsp+rsi*x+dsp]
x64_stat x64_movsd_xrmob(size_t *p, uint8_t *m, reg d, reg s, reg o, scale x, uint8_t dsp);

// lea rax, [rax+dsp8]
x64_stat x64_lea_rrmb(size_t *p, uint8_t *m, reg d, reg s, uint8_t dsp);

// inc rax
x64_stat x64_inc_r(size_t *p, uint8_t *m, reg r);

// inc qword ptr[rax]
x64_stat x64_inc_rm(size_t *p, uint8_t *m, reg r);

// inc qword ptr[rax+dsp8]
x64_stat x64_inc_rmb(size_t *p, uint8_t *m, reg r, uint8_t dsp);

// add rax, byte
x64_stat x64_add_rb(size_t *p, uint8_t *m, reg r, uint8_t b);

// add qword prt[rax], byte
x64_stat x64_add_rmb(size_t *p, uint8_t *m, reg r, uint8_t b);

// add qword prt[rax + dsp8], byte
x64_stat x64_add_rmbb(size_t *p, uint8_t *m, reg r, uint8_t dsp, uint8_t b);

// add rax, rdi
x64_stat x64_add_rr(size_t *p, uint8_t *m, reg d, reg s);

// add qword ptr[rsp] rax
x64_stat x64_add_rmr(size_t *p, uint8_t *m, reg d, reg s);

// add qword ptr[rsp+dsp8] rax
x64_stat x64_add_rmbr(size_t *p, uint8_t *m, reg d, uint8_t dsp, reg s);

// addsd xmm0, xmm1
x64_stat x64_addsd_xx(size_t *p, uint8_t *m, reg d, reg s);

// addsd xmm0, qword ptr[rip+dsp32]
x64_stat x64_addsd_xi(size_t *p, uint8_t *m, reg d, uint32_t dsp);

// dec rax
x64_stat x64_dec_r(size_t *p, uint8_t *m, reg r);

// dec qword ptr[rax]
x64_stat x64_dec_rm(size_t *p, uint8_t *m, reg r);

// dec qword ptr[rax+dsp8]
x64_stat x64_dec_rmb(size_t *p, uint8_t *m, reg r, uint8_t dsp);

// sub rax, byte
x64_stat x64_sub_rb(size_t *p, uint8_t *m, reg r, uint8_t b);

// sub rax, rdi
x64_stat x64_sub_rr(size_t *p, uint8_t *m, reg d, reg s);

// subsd xmm0, xmm1
x64_stat x64_subsd_xx(size_t *p, uint8_t *m, reg d, reg s);

// subsd xmm0, qword ptr[rip+dsp32]
x64_stat x64_subsd_xi(size_t *p, uint8_t *m, reg d, uint32_t dsp);

// neg rax
x64_stat x64_neg_r(size_t *p, uint8_t *m, reg r);

// mul rsi
x64_stat x64_mul_r(size_t *p, uint8_t *m, reg r);

// imul rsi
x64_stat x64_imul_r(size_t *p, uint8_t *m, reg r);

// imul qword ptr [rsp]
x64_stat x64_imul_rm(size_t *p, uint8_t *m, reg r);

// imul qword ptr [rsp+dsp8]
x64_stat x64_imul_rmb(size_t *p, uint8_t *m, reg r, uint8_t dsp);

// div rsi
x64_stat x64_div_r(size_t *p, uint8_t *m, reg r);

// idiv rsi
x64_stat x64_idiv_r(size_t *p, uint8_t *m, reg r);

// mulsd xmm0, xmm1
x64_stat x64_mulsd_xx(size_t *p, uint8_t *m, reg d, reg s);

// mulsd xmm0, qword ptr[rip+dsp32]
x64_stat x64_mulsd_xi(size_t *p, uint8_t *m, reg d, uint32_t dsp);

// divsd xmm0, xmm1
x64_stat x64_divsd_xx(size_t *p, uint8_t *m, reg d, reg s);

// cvtsi2sd xmm0, rax
x64_stat x64_cvtsi2sd_xr(size_t *p, uint8_t *m, reg d, reg s);

// and rax, rdi
x64_stat x64_and_rr(size_t *p, uint8_t *m, reg d, reg s);

// pxor xmm0, xmm1
x64_stat x64_pxor_xx(size_t *p, uint8_t *m, reg d, reg s);

// xor rax, rdi
x64_stat x64_xor_rr(size_t *p, uint8_t *m, reg d, reg s);

// cmp rax, rdi
x64_stat x64_cmp_rr(size_t *p, uint8_t *m, reg d, reg s);

// cmp rax, qword ptr[rsp]
x64_stat x64_cmp_rrm(size_t *p, uint8_t *m, reg d, reg s);

// cmp rax quowrd ptr[rsp+dsp8]
x64_stat x64_cmp_rrmb(size_t *p, uint8_t *m, reg d, reg s, uint8_t dsp);

// cmp rax qword ptr[rsp+dsp32]
x64_stat x64_cmp_rrmd(size_t *p, uint8_t *m, reg d, reg s, uint32_t dsp);

// cmp rax, qword ptr[rip+dsp32]
x64_stat x64_cmp_ri(size_t *p, uint8_t *m, reg r, uint32_t dsp);

// cmp rax, imm8
x64_stat x64_cmp_rb(size_t *p, uint8_t *m, reg r, uint8_t b);

// cmp qword ptr[rax], imm8
x64_stat x64_cmp_rmb(size_t *p, uint8_t *m, reg r, uint8_t b);

// cmp qword ptr[rax+dsp8], imm8
x64_stat x64_cmp_rmbb(size_t *p, uint8_t *m, reg r, uint8_t dsp, uint8_t b);

// cmp rax, imm32
x64_stat x64_cmp_rd(size_t *p, uint8_t *m, reg r, uint32_t d);

// ucomisd xmm0, xmm1
x64_stat x64_ucomisd_xx(size_t *p, uint8_t *m, reg r, reg s);

// ucomisd xmm0, aword ptr[rip+disp32]
x64_stat x64_ucomisd_xi(size_t *p, uint8_t *m, reg d, uint32_t dsp);

// comisd xmm0, xmm1
x64_stat x64_comisd_xx(size_t *p, uint8_t *m, reg d, reg s);

// comisd xmm0, aword ptr[rip+disp32]
x64_stat x64_comisd_xi(size_t *p, uint8_t *m, reg r, uint32_t dsp);

// test rax, rdi
x64_stat x64_test_rr(size_t *p, uint8_t *m, reg d, reg s);

// jump up byte
uint8_t x64_jmpu_lblb(size_t from, size_t to);

// jump up dword
uint32_t x64_jmpu_lbld(size_t from, size_t to);

// get current btye
uint8_t *x64_lb(size_t p, uint8_t *m);

// jump down byte
void x64_jmpd_lblb(uint8_t *byte, size_t from, size_t to);

// jump down dword
void x64_jmpd_lbld(uint8_t *m, size_t from, size_t to);

// jmp byte
x64_stat x64_jmp_b(size_t *p, uint8_t *m, uint8_t b);

// jmp dword
x64_stat x64_jmp_d(size_t *p, uint8_t *m, uint32_t dw);

// jb jnae jc byte
x64_stat x64_jbjnaejc_b(size_t *p, uint8_t *m, uint8_t b);

// jb jnae jc dword
x64_stat x64_jbjnaejc_d(size_t *p, uint8_t *m, uint32_t dw);

// jnb jae jnc byte
x64_stat x64_jnbjaejnc_b(size_t *p, uint8_t *m, uint8_t b);

// jnb jae jnc dword
x64_stat x64_jnbjaejnc_d(size_t *p, uint8_t *m, uint32_t dw);

// jz je btye
x64_stat x64_jzje_b(size_t *p, uint8_t *m, uint8_t b);

// jz je dword
x64_stat x64_jzje_d(size_t *p, uint8_t *m, uint32_t dw);

// jnz jne byte
x64_stat x64_jnzjne_b(size_t *p, uint8_t *m, uint8_t b);

// jnz jne dword
x64_stat x64_jnzjne_d(size_t *p, uint8_t *m, uint32_t dw);

// jbe jna byte
x64_stat x64_jbejna_b(size_t *p, uint8_t *m, uint8_t b);

// jbe jna dword
x64_stat x64_jbejna_d(size_t *p, uint8_t *m, uint32_t dw);

// jnbe ja byte
x64_stat x64_jnbeja_b(size_t *p, uint8_t *m, uint8_t b);

// jnbe ja dword
x64_stat x64_jnbeja_d(size_t *p, uint8_t *m, uint32_t dw);

// jl jnge byte
x64_stat x64_jljnge_b(size_t *p, uint8_t *m, uint8_t b);

// jl jnge dword
x64_stat x64_jljnge_d(size_t *p, uint8_t *m, uint32_t dw);

// jnl jge byte
x64_stat x64_jnljge_b(size_t *p, uint8_t *m, uint8_t b);

// jnl jge dword
x64_stat x64_jnljge_d(size_t *p, uint8_t *m, uint32_t dw);

// jle jng byte
x64_stat x64_jlejng_b(size_t *p, uint8_t *m, uint8_t b);

// jle jng dword
x64_stat x64_jlejng_d(size_t *p, uint8_t *m, uint32_t dw);

// jnle jg byte
x64_stat x64_jnlejg_b(size_t *p, uint8_t *m, uint8_t b);

// jnle jg dword
x64_stat x64_jnlejg_d(size_t *p, uint8_t *m, uint32_t dw);

// loop byte
x64_stat x64_loop_b(size_t *p, uint8_t *m, uint8_t b);

// setl setnge rax
x64_stat x64_setlsetnge_r(size_t *p, uint8_t *m, reg r);
