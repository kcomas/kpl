
#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct __tests _tests;

extern bool _tl; // test lock, only run one test

extern _tests *__t;

typedef void _test_fn(_tests *t);

typedef struct __tests {
    int ln; // failed line
    const char *m, *sf, *ef; // msg, source file, error file
    _test_fn *tf;
    struct __tests *nt;
    char n[]; // name
} _tests;

void _a(const char *name, const char *sf, _test_fn *tf);

#define A(S, M) do { \
    if (!(S)) { \
        if (!_t->ln) { \
            _t->ln = __LINE__; \
            _t->m = M; \
            _t->ef = __FILE__; \
        } \
        return; \
    } \
} while (0)

#define E() do { \
    if (_t->ln) return; \
} while (0)

#define _TF(N) static void test_##N(__attribute__((unused)) _tests *_t)

#define T(N) _TF(N); \
    static __attribute__((constructor)) void __##N(void) { \
        if (!_tl) _a(#N, __FILE__, test_##N); \
    } \
_TF(N)

// run only one test for adding and editing tests
#define TO(N) _TF(N); \
    static __attribute__((constructor)) void __##N(void) { \
        if (_tl) { \
            printf("\e[1;91mTO CAN ONLY BE USED ONCE\e[0m\n"); \
            exit(1); \
        } \
        _tl = true; \
        while (__t) { \
            _tests *tt = __t; \
            __t = __t->nt; \
            free(tt); \
        } \
        _a(#N, __FILE__, test_##N); \
    } \
_TF(N)
