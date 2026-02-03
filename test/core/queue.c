
#include "../test.h"

TEST(core_queue_print) {
    core_queue queue;
    core_queue_init(&queue, CORE_QUEUE_ITEM_PRINT(NL_END) | CORE_QUEUE_ITEM_PRINT(DEPENDENCIES));
    ASSERT(queue.ma, "should not be null");
    core_queue_item *fib_load = core_queue_add(&queue, nullptr, "./example/fib_load.kpl");
    ASSERT(fib_load, "should not be null");
    core_queue_item *fib = core_queue_add(&queue, fib_load->filename->data, "./fib.kpl");
    ASSERT(fib, "should not be null");
    core_queue_item_add_parent(fib, fib_load);
    core_queue_item *fib2 = core_queue_add(&queue, fib_load->filename->data, "./fib.kpl");
    ASSERT(fib == fib2, "should be same ptr");
    core_queue_item_add_parent(fib2, fib_load);
    map_print(queue.ma, stdout, 0, MAP_PRINT(_));
    core_queue_free(&queue);
}
