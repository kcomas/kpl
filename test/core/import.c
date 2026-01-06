
#include "../test.h"

TEST(core_import) {
    core_queue queue;
    core_queue_init(&queue, QUEUE_ITEM_PRINT(NL_END) | QUEUE_ITEM_PRINT(DEPENDENCIES));
    core_queue_item *fib_load = core_queue_add(&queue, nullptr, "./example/fib_load.kpl");
    ASSERT(fib_load && queue.state_count.init == 1, "fib load should be in init state");
    core_import_before(fib_load);
    ASSERT(fib_load->dependencies == 1, "item deps should been 1 for lock");
    error *er = core_import(fib_load);
    if (er)
        error_print(er, stdout, 0, ERROR_PRINT(NL_END));
    ASSERT(!er, "import should not error");
    ASSERT(fib_load->dependencies == 1, "item deps should been 1 for new dependencies");
    ASSERT(queue.state_count.init == 1 && queue.state_count.dependencies == 1, "dependencies should be added");
    map_print(queue.ma, stdout, 0, MAP_PRINT(_));
    core_queue_free(&queue);
}
