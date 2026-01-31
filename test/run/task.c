
#include "../test.h"

atomic_size_t counter = 0;

error *task_counter_fn(def_data data) {
    (void) data;
    counter++;
    return nullptr;
}

#define TEST_THREADS THREADS

TEST(task_basic_join) {
    counter = 0;
    task *tasks[TEST_THREADS] = {};
    for (size_t task_idx = 0; task_idx < TEST_THREADS; task_idx++)
        tasks[task_idx] = task_init(0, &def_unused_fn_table, def_u64(task_idx), task_counter_fn);
    for (size_t task_idx = 0; task_idx < TEST_THREADS; task_idx++) {
        result re = task_join(tasks[task_idx]);
        ASSERT(!re.er && re.value.u64 == task_idx, "should not error");
    }
    ASSERT(counter == TEST_THREADS, "invalid counter value");
}

TEST(task_basic_detatch) {
    counter = 0;
    for (size_t task_idx = 0; task_idx < TEST_THREADS; task_idx++)
        task_init_detached(0, &def_unused_fn_table, def_u64(task_idx), task_counter_fn);
    ASSERT(counter > 0, "should have some increments");
}
