
#include "test.h"

static size_t test_len = 0;

static test test_array[TEST_MAX_COUNT] = {};

void test_add(const char *name, test_fn fn) {
    if (test_len == TEST_MAX_COUNT) {
        printf(COLOR2(BOLD, RED) "Max Test Count Reached Increase TEST_MAX_COUNT\n" COLOR(RESET));
        exit(DEF_EXIT_ERROR);
    }
    test_array[test_len++] = (test) { .item = { .name = name, .fn = fn } };
}

int main(void) {
    size_t pass_count = 0, fail_count = 0;
    const char *name;
    for (size_t test_idx = 0; test_idx < test_len; test_idx++) {
        name = test_array[test_idx].item.name;
        printf("TEST %s\n", name);
        if ((test_array[test_idx].er = test_array[test_idx].item.fn())) {
            error_print(test_array[test_idx].er, stdout, 0, ERROR_PRINT(NL_END));
            printf(COLOR(RED) "FAIL %s\n" COLOR(RESET), name);
            fail_count++;
        } else {
            printf(COLOR(GREEN) "PASS %s\n" COLOR(RESET), name);
            test_array[test_idx].er = NULL;
            pass_count++;
        }
    }
    if (fail_count)
        printf(COLOR2(BOLD, LIGHT_RED) "FAIL " COLOR(RESET));
    else
        printf(COLOR2(BOLD, LIGHT_GREEN) "PASS " COLOR(RESET));
    printf(COLOR(GREEN) "%lu " COLOR(RED) "%lu" COLOR(RESET) "\n", pass_count, fail_count);
    for (size_t test_idx = 0; test_idx < test_len; test_idx++) {
        if (test_array[test_idx].er) {
            error_print(test_array[test_idx].er, stdout, 0, ERROR_PRINT(NL_END));
            error_free(test_array[test_idx].er);
        }
    }
    return fail_count ? DEF_EXIT_ERROR : 0;
}
