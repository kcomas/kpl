
#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef void *alfn(size_t s);

typedef void frfn(void *v);

typedef struct {
    uint8_t a, b, c, d;
} c4;

typedef union {
    bool b;
    c4 c;
    uint8_t u3;
    uint16_t u4;
    uint32_t u5;
    uint64_t u6;
    int8_t i3;
    int16_t i4;
    int32_t i5;
    int64_t i6;
    float f;
    double d;
    void *p;
} var;

#define VAR(T, D) ((var) { .T = D })

#define B(D) VAR(b, D)
#define C4(A, B, C, D) VAR(c, (c4) { .a = A, .b = B, .c = C, .d = D })
#define C1(A) C4(A, '\0', '\0', '\0')
#define C2(A, B) C4(A, B, '\0', '\0')
#define C3(A, B, C) C4(A, B, C, '\0')
#define U3(D) VAR(u3, D)
#define U4(D) VAR(u4, D)
#define U5(D) VAR(u5, D)
#define U6(D) VAR(u6, D)
#define I3(D) VAR(i3, D)
#define I4(D) VAR(i4, D)
#define I5(D) VAR(i5, D)
#define I6(D) VAR(i6, D)
#define F(D) VAR(f, D)
#define D(D) VAR(d, D)
#define P(D) VAR(p, (void*) D)
