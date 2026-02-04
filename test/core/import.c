
#include "../test.h"

TEST(core_import) {
    core_queue queue;
    core_queue_init(&queue, CORE_QUEUE_ITEM_PRINT(NL_END) | CORE_QUEUE_ITEM_PRINT(DEPENDENCIES));
    core_queue_item *fib_load = core_queue_add(&queue, nullptr, "./example/fib_import.kpl");
    fib_load->state_flags |= CORE_QUEUE_ITEM_FLAG(MAIN);
    core_import_before(fib_load);
    ASSERT(fib_load->dependencies == 1, "item deps should been 1 for lock");
    error *er = core_import(&queue, fib_load);
    if (er)
        error_print(er, stdout, 0, ERROR_PRINT(NL_END));
    ASSERT(!er, "import should not error");
    ASSERT(fib_load->dependencies == 1, "item deps should been 1 for new dependencies");
    map_print(queue.ma, stdout, 0, MAP_PRINT(_));
    core_queue_free(&queue);
}
