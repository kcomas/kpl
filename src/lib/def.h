
#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

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
    float f;
    double d;
    void *p;
} un;

#define UN(T, D) ((un) { .T = D })
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
#define F(D) UN(f, D)
#define D(D) UN(d, D)
#define P(D) UN(p, (void*) D)

// get next utf8 char starting at s
un c4_g(const char *const str, size_t s, size_t *e);

bool c4_eq(un a, un b);
