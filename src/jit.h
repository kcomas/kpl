
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#define R(N) R_##N

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
    R(15)
} rregs;

#define X(N) X_##N

typedef enum {
    X(0) = 16,
    X(1),
    X(2)
    X(3)
    X(4)
    X(5)
    X(6)
    X(7)
    X(8)
    X(9)
    X(10)
    X(11)
    X(12)
    X(13)
    X(14)
    X(15)
} xregs;

// 0-8 gen lower, 9-15 // gen upper
// 16-24 xmm lower 24-31 xmm upper

void jit_a(size_t *pos, uint8_t *m, uint8_t b);

void jit_b(size_t *pos, uint8_t *m, size_t len, ...);

void jit_c(size_t *pos, uint8_t *m, size_t len, uint8_t b[]);
