
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
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

// 0-7 gen lower, 8-15 // gen upper
// 16-23 xmm lower 24-32 xmm upper

#define REX(N) REX_##N

typedef enum {
    REX(B) = 0x41,
    REX(R) = 0x44,
    REX(W) = 0x48
} rex;

#define JIT_STAT(N) JIT_STAT_##N

typedef enum {
    JIT_STAT(OK),
    JIT_STAT(INV_REG)
} jit_stat;

uint8_t *jit_mmap(size_t size);

void jit_munmap(size_t size, uint8_t *m);

jit_stat jit_a(size_t *p, uint8_t *m, uint8_t b);

jit_stat jit_b(size_t *p, uint8_t *m, size_t len, ...);

jit_stat jit_c(size_t *p, uint8_t *m, size_t len, uint8_t b[]);

jit_stat jit_d(size_t *p, uint8_t *m, void *v);

// nop
jit_stat jit_nop(size_t *p, uint8_t *m);

// ret
jit_stat jit_ret(size_t *p, uint8_t *m);

// leave
jit_stat jit_leave(size_t *p, uint8_t *m);

// push rax
jit_stat jit_push(size_t *p, uint8_t *m, reg r);

// pop rax
jit_stat jit_pop(size_t *p, uint8_t *m, reg r);

jit_stat jit_movrq(size_t *p, uint8_t *m, reg r, void *v);

// call rax
jit_stat jit_callr(size_t *p, uint8_t *m, reg r);

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

// inc rax
jit_stat jit_incr(size_t *p, uint8_t *m, reg r);

// add rax, byte
jit_stat jit_addrb(size_t *p, uint8_t *m, reg r, int8_t b);

// add rax, rdi
jit_stat jit_addrr(size_t *p, uint8_t *m, reg d, reg s);

// dec rax
jit_stat jit_decr(size_t *p, uint8_t *m, reg r);

// sub rax, byte
jit_stat jit_subrb(size_t *p, uint8_t *m, reg r, int8_t b);

// sub rax, rdi
jit_stat jit_subrr(size_t *p, uint8_t *m, reg d, reg s);

// xor rax, rdi
jit_stat jit_xorrr(size_t *p, uint8_t *m, reg d, reg s);

// cmp rax, rdi
jit_stat jit_cmprr(size_t *p, uint8_t *m, reg d, reg s);

// test rax, rdi
jit_stat jit_testrr(size_t *p, uint8_t *m, reg d, reg s);

// jump down
uint8_t jit_jmpu_lblb(size_t from, size_t to);

// get current btye
uint8_t *jit_lb(size_t p, uint8_t *m);

// jump up
void jit_jmpd_lblb(uint8_t *byte, size_t from, size_t to);

// jmp byte
jit_stat jit_jmpb(size_t *p, uint8_t *m, uint8_t b);

// jc byte
jit_stat jit_jcb(size_t *p, uint8_t *m, uint8_t b);

// jne byte
jit_stat jit_jneb(size_t *p, uint8_t *m, uint8_t b);

// jna byte
jit_stat jit_jnab(size_t *p, uint8_t *m, uint8_t b);
