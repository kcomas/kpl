
#include "../test.h"

atomic_size_t counter = 0;

error *task_nop_fn(def_data data) {
    (void) data;
    counter++;
    return NULL;
}

#define TEST_THREADS THREADS

TEST(task_basic_join) {
    counter = 0;
    task *tasks[TEST_THREADS] = {};
    for (size_t task_idx = 0; task_idx < TEST_THREADS; task_idx++)
        tasks[task_idx] = task_init(0, &def_unused_fn_table, DEF_U64(task_idx), task_nop_fn);
    for (size_t task_idx = 0; task_idx < TEST_THREADS; task_idx++)
        task_join(tasks[task_idx]);
    ASSERT(counter == TEST_THREADS, "invalid counter value");
}

TEST(task_basic_free) {
    counter = 0;
    task *tasks[TEST_THREADS] = {};
    for (size_t task_idx = 0; task_idx < TEST_THREADS; task_idx++)
        tasks[task_idx] = task_init(0, &def_unused_fn_table, DEF_U64(task_idx), task_nop_fn);
    for (size_t task_idx = 0; task_idx < TEST_THREADS; task_idx++)
        task_free(tasks[task_idx]);
    ASSERT(counter > 0, "should have some increments");
}
