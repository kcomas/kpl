
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct __tests _tests;

typedef void _test_fn(_tests *t);

typedef struct __tests {
    int ln; // failed line
    const char *m;
    _test_fn *tf;
    struct __tests *n;
    char name[];
} _tests;

void _a(const char *name, _test_fn *tf);

#define I const char *_f = __FILE__; \
    _tests *_t = NULL;

#define A(S, M) do { \
    if (!(S)) { \
        t->ln = __LINE__; \
        t->m = M; \
        return; \
    } \
} while (0)

#define T(N, B) __attribute__((constructor)) void N##__(void) { \
    void N##_(__attribute__((unused)) _tests *t) B; \
    _a(#N, N##_); \
}
