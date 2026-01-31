
#pragma once

#include "../src/core/core.h"

typedef struct _test test;

typedef void test_fn(test *_test);

#ifndef TEST_MAX_COUNT
#define TEST_MAX_COUNT 1000
#endif

typedef struct _test {
    const char *name;
    union {
        test_fn *fn;
        error *er;
    } run;
} test;

void test_add(const char *name, test_fn *fn);

#define _TEST_FN(NAME) static void test_##NAME([[maybe_unused]] test *_test)

#define TEST(NAME) _TEST_FN(NAME); \
[[gnu::constructor]] static void _test_##NAME(void) { \
    test_add(#NAME, test_##NAME); \
} \
_TEST_FN(NAME)

#define _FAIL(MSG) _test->run.er = ERROR_INIT(0, &def_unused_fn_table, def(), MSG); return

#define ASSERT(STMT, MSG) do { \
    if (!(STMT)) { \
        _FAIL(MSG); \
    } \
} while (0)

#define FAIL(MSG) do { \
    _FAIL(MSG); \
} while (0)
