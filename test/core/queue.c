
#include "../test.h"

TEST(core_queue_print) {
    map *queue = queue_init(QUEUE_ITEM_PRINT(NL_END) | QUEUE_ITEM_PRINT(DEPENDENCIES));
    ASSERT(queue, "should not be null");
    queue_item *fib_load = queue_add(&queue, NULL, "./example/fib_load.kpl");
    ASSERT(fib_load, "should not be null");
    queue_item *fib = queue_add(&queue, fib_load->filename->data, "./fib.kpl");
    ASSERT(fib, "should not be null");
    queue_item_add_parent(fib, fib_load);
    queue_item *fib2 = queue_add(&queue, fib_load->filename->data, "./fib.kpl");
    ASSERT(fib == fib2, "should be same ptr");
    map_print(queue, stdout, 0, MAP_PRINT(_));
    map_free(queue);
}
