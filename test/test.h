
#pragma once

#include "../src/run/run.h"

typedef error *test_fn(void);

#ifndef TEST_MAX_COUNT
#define TEST_MAX_COUNT 1000
#endif

typedef union {
    error *er;
    struct {
        test_fn *fn;
        const char *name;
    } item;
} test;

void test_add(const char *name, test_fn *fn);

#define _TEST_FN(NAME) static error *test_##NAME(void)

#define TEST(NAME) _TEST_FN(NAME); \
[[gnu::constructor]] static void _test_##NAME(void) { \
    test_add("test_" #NAME, test_##NAME); \
} \
_TEST_FN(NAME)

#define ASSERT(STMT, MSG) do { \
    if (!(STMT)) \
        return ERROR_INIT(NULL, 0, NULL, DEF(_), MSG); \
} while (0)
