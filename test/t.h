
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct __tests _tests;

typedef void _test_fn(_tests *t);

typedef struct __tests {
    int ln; // failed line
    const char *m, *f; // msg, file
    _test_fn *tf;
    struct __tests *nt;
    char n[]; // name
} _tests;

void _a(const char *name, _test_fn *tf);

#define I const char *__f = __FILE__; \
    _tests *__t = NULL;

#define A(S, M) do { \
    if (!(S)) { \
        if (!_t->ln) { \
            _t->ln = __LINE__; \
            _t->m = M; \
            _t->f = __FILE__; \
        } \
        return; \
    } \
} while (0)

#define T(N, ...)  void _##N(__attribute__((unused)) _tests *_t) __VA_ARGS__; \
    __attribute__((constructor)) void __##N(void) { \
    _a(#N, &_##N); \
}
