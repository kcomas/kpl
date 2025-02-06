
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

#define I const char *__f = __FILE__; \
    _tests *__t = NULL;

#define A(S, M) do { \
    if (!(S)) { \
        _t->ln = __LINE__; \
        _t->m = M; \
        return; \
    } \
} while (0)

#define T(N, B) __attribute__((constructor)) void __##N(void) { \
    void _##N(__attribute__((unused)) _tests *_t) B; \
    _a(#N, _##N); \
}
