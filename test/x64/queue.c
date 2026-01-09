
#include "../test.h"

TEST(x64_queue_resolve_item) {
    printf("Queue Resolve Item Size " COLOR(BOLD) "%lu\n" COLOR(RESET), sizeof(x64_queue_resolve_item));
    const int32_t byte_idx = 1234;
    const x64_op_reg op_size = X64_OP(REL32);
    def_data data = x64_queue_resolve_item_encode(byte_idx, op_size);
    x64_queue_resolve_item item = x64_queue_resolve_item_decode(data);
    ASSERT(item.byte_idx == byte_idx && item.op_size == op_size, "invalid resolve encode");
}

TEST(x64_asm_queue) {
    map *queue = x64_queue_asm_init();
    ASSERT(x64_queue_add(&queue, -1, 1, 10, X64_OP(REL32)) == DEF_STATUS(OK), "invalid queue add");
    ASSERT(x64_queue_add(&queue, -1, 1, 20, X64_OP(REL32)) == DEF_STATUS(OK), "invalid queue add");
    ASSERT(x64_queue_add(&queue, -1, 2, 30, X64_OP(DSP32)) == DEF_STATUS(OK), "invalid queue add");
    map_print(queue, stdout, 0, MAP_PRINT(_));
    ASSERT(queue->used == 2, "invalid number of entries");
    map_free(queue);
}
