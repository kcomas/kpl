
#include "../test.h"

error *task_nop_fn(def_data data) {
    (void) data;
    return NULL;
}

#define TEST_THREADS THREADS

TEST(task_basic_join) {
    task *tasks[TEST_THREADS] = {};
    for (size_t task_idx = 0; task_idx < TEST_THREADS; task_idx++)
        tasks[task_idx] = task_init(0, &def_unused_fn_table, DEF_U64(task_idx), task_nop_fn);
    for (size_t task_idx = 0; task_idx < TEST_THREADS; task_idx++)
        task_join(tasks[task_idx]);
}

TEST(task_basic_free) {
    task *tasks[TEST_THREADS] = {};
    for (size_t task_idx = 0; task_idx < TEST_THREADS; task_idx++)
        tasks[task_idx] = task_init(0, &def_unused_fn_table, DEF_U64(task_idx), task_nop_fn);
    for (size_t task_idx = 0; task_idx < TEST_THREADS; task_idx++)
        task_free(tasks[task_idx]);
}
