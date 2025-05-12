
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#define HERE(MSG) printf("\e[1m%s:%d\e[0m \e[1;94m%s\e[0m \e[1;33m%s\e[0m\n", __FILE__, __LINE__, __FUNCTION__, MSG);

#define STOP(MSG) do { \
        printf("\e[1;91m%s\e[0m\n", MSG); \
        exit(1); \
} while (0)

typedef void *alfn(size_t s);

typedef void frfn(void *v);

// utf8 char
typedef struct {
    uint8_t a, b, c, d;
} c4;

// union of types
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
    float f5;
    double f6;
    void *p;
} un;

#define _UOGH(CT, T) CT T##_g_o(un u, size_t o)

#define _UOSH(CT, T) un T##_s_o(un u, size_t o, CT v)

#define _UOH(CT, T) _UOGH(CT, T); \
    _UOSH(CT, T)

_UOH(uint8_t, u3);
_UOH(int8_t, i3);
_UOH(uint16_t, u4);
_UOH(int16_t, i4);
_UOH(uint32_t, u5);
_UOH(int32_t, i5);
_UOH(float, f5);

#define UN(T, D) ((un) { .T = (D) })
#define B(D) UN(b, D)
#define C4(A, B, C, D) UN(c, ((c4) { .a = A, .b = B, .c = C, .d = D }))
#define C3(A, B, C) C4(A, B, C, '\0')
#define C2(A, B) C3(A, B, '\0')
#define C1(A) C2(A, '\0')
#define U3(D) UN(u3, D)
#define U4(D) UN(u4, D)
#define U5(D) UN(u5, D)
#define U6(D) UN(u6, D)
#define I3(D) UN(i3, D)
#define I4(D) UN(i4, D)
#define I5(D) UN(i5, D)
#define I6(D) UN(i6, D)
#define F5(D) UN(f5, D)
#define F6(D) UN(f6, D)
#define P(D) UN(p, (void*) D)

#define _UN_OP_H(S, O) un S##_##O(un a, un b)

#define _UN_OP_HH(O) _UN_OP_H(u3, O); \
    _UN_OP_H(u4, O); \
    _UN_OP_H(u5, O); \
    _UN_OP_H(u6, O); \
    _UN_OP_H(i3, O); \
    _UN_OP_H(i4, O); \
    _UN_OP_H(i5, O); \
    _UN_OP_H(i6, O); \
    _UN_OP_H(f5, O); \
    _UN_OP_H(f6, O)

_UN_OP_HH(add);
_UN_OP_HH(sub);
_UN_OP_HH(mul);
_UN_OP_HH(div);

// get next utf8 char starting at s
un c4_g(const char *str, size_t s, size_t *e);

bool c4_eq(un a, un b);

typedef struct {
    alfn *a;
    frfn *f;
} alfr;
