
#pragma once

#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include "def.h"

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

// map page aligned
uint8_t *x64_mmap(size_t size);

// unmap page aligned
void x64_munmap(size_t size, uint8_t *m);

x64_stat x64_a(size_t *p, uint8_t *m, uint8_t b);

x64_stat x64_b(size_t *p, uint8_t *m, size_t len, ...);

x64_stat x64_c(size_t *p, uint8_t *m, size_t len, uint8_t b[]);

x64_stat x64_d(size_t *p, uint8_t *m, un u);

x64_stat x64_e(size_t *p, uint8_t *m, uint8_t size, un v);

// nop
x64_stat x64_nop(size_t *p, uint8_t *m);

// ret
x64_stat x64_ret(size_t *p, uint8_t *m);

// leave
x64_stat x64_leave(size_t *p, uint8_t *m);

// push rmx
x64_stat x64_push_r(size_t *p, uint8_t *m, reg r);

// pop rmx
x64_stat x64_pop_r(size_t *p, uint8_t *m, reg r);

// call rmx
x64_stat x64_call_r(size_t *p, uint8_t *m, reg r);

// call dword
x64_stat x64_call_dw(size_t *p, uint8_t *m, uint32_t dw);

// mov rmx, 64bit value
x64_stat x64_mov_rq(size_t *p, uint8_t *m, reg r, un u);

// mov rmx, rdi
x64_stat x64_mov_rr(size_t *p, uint8_t *m, reg d, reg s);

// mov qword ptr[rdi], rmx
x64_stat x64_mov_rmr(size_t *p, uint8_t *m, reg d, reg s);

// mov qword ptr[rdi+dsp8], rmx
x64_stat x64_mov_rmbr(size_t *p, uint8_t *m, reg d, uint8_t dsp, reg s);

// mov rmx, qword ptr [rdi]
x64_stat x64_mov_rrm(size_t *p, uint8_t *m, reg d, reg s);

// mov rmx, qword ptr[rdi+dsp8]
x64_stat x64_mov_rrmb(size_t *p, uint8_t *m, reg d, reg s, uint8_t dsp);

// lea rmx, [rmx+dsp8]
x64_stat x64_lea_rrb(size_t *p, uint8_t *m, reg d, reg s, uint8_t dsp);

// inc rmx
x64_stat x64_inc_r(size_t *p, uint8_t *m, reg r);

// add rmx, byte
x64_stat x64_add_rb(size_t *p, uint8_t *m, reg r, int8_t b);

// add rmx, rdi
x64_stat x64_add_rr(size_t *p, uint8_t *m, reg d, reg s);

// addsd xmm0, xmm1
x64_stat x64_addsd_rr(size_t *p, uint8_t *m, reg d, reg s);

// dec rmx
x64_stat x64_dec_r(size_t *p, uint8_t *m, reg r);

// sub rmx, byte
x64_stat x64_sub_rb(size_t *p, uint8_t *m, reg r, int8_t b);

// sub rmx, rdi
x64_stat x64_sub_rr(size_t *p, uint8_t *m, reg d, reg s);

// subsd xmm0, xmm1
x64_stat x64_subsd_rr(size_t *p, uint8_t *m, reg d, reg s);

// and rmx, rdi
x64_stat x64_and_rr(size_t *p, uint8_t *m, reg d, reg s);

// xor rmx, rdi
x64_stat x64_xor_rr(size_t *p, uint8_t *m, reg d, reg s);

// cmp rmx, rdi
x64_stat x64_cmp_rr(size_t *p, uint8_t *m, reg d, reg s);

// cmp rmx, qword ptr[rsp]
x64_stat x64_cmp_rrm(size_t *p, uint8_t *m, reg d, reg s);

// test rmx, rdi
x64_stat x64_test_rr(size_t *p, uint8_t *m, reg d, reg s);

// jump up byte
uint8_t x64_jmpu_lblb(size_t from, size_t to);

// jump up dword
uint32_t x64_jmpu_lbldw(size_t from, size_t to);

// get current btye
uint8_t *x64_lb(size_t p, uint8_t *m);

// jump down byte
void x64_jmpd_lblb(uint8_t *byte, size_t from, size_t to);

// jump down dword
void x64_jmpd_lbldw(uint8_t *m, size_t from, size_t to);

// jmp byte
x64_stat x64_jmp_b(size_t *p, uint8_t *m, uint8_t b);

// jmp dword
x64_stat x64_jmp_dw(size_t *p, uint8_t *m, uint32_t dw);

// jb jnae jc byte
x64_stat x64_jbjnaejc_b(size_t *p, uint8_t *m, uint8_t b);

// jb jnae jc dword
x64_stat x64_jbjnaejc_dw(size_t *p, uint8_t *m, uint32_t dw);

// jnb jae jnc byte
x64_stat x64_jnbjaejnc_b(size_t *p, uint8_t *m, uint8_t b);

// jnb jae jnc dword
x64_stat x64_jnbjaejnc_dw(size_t *p, uint8_t *m, uint32_t dw);

// jz je btye
x64_stat x64_jzje_b(size_t *p, uint8_t *m, uint8_t b);

// jz je dword
x64_stat x64_jzje_dw(size_t *p, uint8_t *m, uint32_t dw);

// jnz jne byte
x64_stat x64_jnzjne_b(size_t *p, uint8_t *m, uint8_t b);

// jnz jne dword
x64_stat x64_jnzjne_dw(size_t *p, uint8_t *m, uint32_t dw);

// jbe jna byte
x64_stat x64_jbejna_b(size_t *p, uint8_t *m, uint8_t b);

// jbe jna dword
x64_stat x64_jbejna_dw(size_t *p, uint8_t *m, uint32_t dw);

// jnbe ja byte
x64_stat x64_jnbeja_b(size_t *p, uint8_t *m, uint8_t b);

// jnbe ja dword
x64_stat x64_jnbeja_dw(size_t *p, uint8_t *m, uint32_t dw);

// jl jnge byte
x64_stat x64_jljnge_b(size_t *p, uint8_t *m, uint8_t b);

// jl jnge dword
x64_stat x64_jljnge_dw(size_t *p, uint8_t *m, uint32_t dw);

// jnl jge byte
x64_stat x64_jnljge_b(size_t *p, uint8_t *m, uint8_t b);

// jnl jge dword
x64_stat x64_jnljge_dw(size_t *p, uint8_t *m, uint32_t dw);

// jle jng byte
x64_stat x64_jlejng_b(size_t *p, uint8_t *m, uint8_t b);

// jle jng dword
x64_stat x64_jlejng_dw(size_t *p, uint8_t *m, uint8_t dw);

// jnle jg byte
x64_stat x64_jnlejg_b(size_t *p, uint8_t *m, uint8_t b);

// jnle jg dword
x64_stat x64_jnlejg_dw(size_t *p, uint8_t *m, uint32_t dw);

// setl setnge rax
x64_stat x64_setlsetnge_r(size_t *p, uint8_t *m, reg r);
