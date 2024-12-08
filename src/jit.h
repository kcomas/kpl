
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/mman.h>

#define R(N) R_##N

#define XMM(N) X_##N

typedef enum {
    R(AX) = 0,
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

bool reg_is_upper(reg r);

// 0-7 gen lower, 8-15 // gen upper
// 16-23 xmm lower 24-32 xmm upper

#define REX(N) REX_##N

typedef enum {
    REX(P) = 0x40,
    REX(B) = 0x41,
    REX(W) = 0x48,
    REX(R) = 0x49
} rex;

uint8_t modrm(uint8_t mod, reg d, reg s);

#define JIT_STAT(N) JIT_STAT_##N

typedef enum {
    JIT_STAT(OK),
    JIT_STAT(INV_REG)
} jit_stat;

jit_stat jit_a(size_t *p, uint8_t *m, uint8_t b);

jit_stat jit_b(size_t *p, uint8_t *m, size_t len, ...);

jit_stat jit_c(size_t *p, uint8_t *m, size_t len, uint8_t b[]);

// ret
jit_stat jit_ret(size_t *p, uint8_t *m);

// leave
jit_stat jit_leave(size_t *p, uint8_t *m);

// push rax
jit_stat jit_push(size_t *p, uint8_t *m, reg r);

// pop rax
jit_stat jit_pop(size_t *p, uint8_t *m, reg r);

// mov rax, rdi
jit_stat jit_movrr(size_t *p, uint8_t *m, reg d, reg s);

// mov qword ptr[rdi], rax
jit_stat jit_movrar(size_t *p, uint8_t *m, reg d, reg s);

// mov qword ptr[rdi+dsp8], rax
jit_stat jit_movrabr(size_t *p, uint8_t *m, reg d, uint8_t dsp, reg s);

// mov rax, qword ptr [rdi]
jit_stat jit_movrra(size_t *p, uint8_t *m, reg d, reg s);

// mov rax, qword ptr[rdi+dsp8]
jit_stat jit_movrrab(size_t *p, uint8_t *m, reg d, reg s, uint8_t dsp);

// add rax, b
jit_stat jit_addrb(size_t *p, uint8_t *m, reg r, int8_t b);

// add rax, rdi
jit_stat jit_addrr(size_t *p, uint8_t *m, reg d, reg s);

// sub rax, b
jit_stat jit_subrb(size_t *p, uint8_t *m, reg r, int8_t b);

// sub rax, rdi
jit_stat jit_subrr(size_t *p, uint8_t *m, reg d, reg s);
