
#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef void *alfn(size_t s);

typedef void frfn(void *v);

typedef union {
    bool b;
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
