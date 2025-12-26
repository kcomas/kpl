
#include "test.h"

static size_t test_len = 0;

static test test_array[TEST_MAX_COUNT] = {};

void test_add(const char *name, test_fn fn) {
    if (test_len == TEST_MAX_COUNT) {
        printf(COLOR2(BOLD, RED) "Max Test Count Reached Increase TEST_MAX_COUNT, Exiting\n" COLOR(RESET));
        exit(DEF_EXIT_ERROR);
    }
    test_array[test_len++] = (test) { .name = name, .run = { .fn = fn } };
}

#define ARGC_SKIP 1

static uint64_t argv_mask = 0;

static bool can_run_test(int argc, char *argv[], const char *name) {
    if (argc == 1)
        return true;
    if (!argv_mask)
        return false;
    for (int mask_idx = 1; mask_idx < argc; mask_idx++) {
        if (!(argv_mask & DEF_U64_MASK(mask_idx - ARGC_SKIP)))
            continue;
        if (!strcmp(argv[mask_idx], name)) {
            argv_mask ^= DEF_U64_MASK(mask_idx - ARGC_SKIP);
            return true;
        }
    }
    return false;
}

#define MAX_NAMED_TESTS 64

int main(int argc, char *argv[]) {
    if (argc > MAX_NAMED_TESTS + 1) {
        printf(COLOR2(BOLD, RED) "Max Number Of Named Tests Reached, Max Is %d\n" COLOR(RESET), MAX_NAMED_TESTS);
        return DEF_EXIT_ERROR;
    }
    for (int mask_idx = 1; mask_idx < argc; mask_idx++)
        argv_mask |= DEF_U64_MASK(mask_idx - ARGC_SKIP);
    size_t pass_count = 0, fail_count = 0;
    for (size_t test_idx = 0; test_idx < test_len; test_idx++) {
        test_fn *fn = test_array[test_idx].run.fn;
        test_array[test_idx].run.er = NULL; // must loop though all to null er
        if (!can_run_test(argc, argv, test_array[test_idx].name))
            continue;
        printf("TEST %s\n", test_array[test_idx].name);
        fn(&test_array[test_idx]);
        if (test_array[test_idx].run.er) {
            error_print(test_array[test_idx].run.er, stdout, 0, ERROR_PRINT(NL_END));
            printf(COLOR(RED) "FAIL %s\n" COLOR(RESET), test_array[test_idx].name);
            fail_count++;
        } else {
            printf(COLOR(GREEN) "PASS %s\n" COLOR(RESET), test_array[test_idx].name);
            pass_count++;
        }
    }
    if (fail_count)
        printf(COLOR2(BOLD, LIGHT_RED) "FAIL " COLOR(RESET));
    else
        printf(COLOR2(BOLD, LIGHT_GREEN) "PASS " COLOR(RESET));
    printf(COLOR(GREEN) "%lu " COLOR(RED) "%lu" COLOR(RESET) "\n", pass_count, fail_count);
    for (size_t test_idx = 0; test_idx < test_len; test_idx++)
        if (test_array[test_idx].run.er) {
            printf("%s ", test_array[test_idx].name);
            error_print(test_array[test_idx].run.er, stdout, 0, ERROR_PRINT(NL_END));
            error_free(test_array[test_idx].run.er);
        }
    return fail_count ? DEF_EXIT_ERROR : 0;
}
