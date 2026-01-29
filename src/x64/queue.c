
#include "./queue.h"

x64_queue_size x64_op_to_queue_size(x64_op_reg op) {
    switch (op) {
        case X64_OP(DSP8):
        case X64_OP(REL8):
            return X64_QUEUE_SIZE(8);
        case X64_OP(DSP32):
        case X64_OP(REL32):
            return X64_QUEUE_SIZE(32);
        default:
            break;
    }
    return X64_QUEUE_SIZE(_);
}

def_data x64_queue_resolve_item_encode(int32_t byte_idx, x64_queue_size byte_size) {
    uint64_t value = byte_idx;
    value <<= 32;
    value |= byte_size;
    return DEF_U64(value);
}

x64_queue_resolve_item x64_queue_resolve_item_decode(def_data data) {
    uint64_t value = data.u64;
    return (x64_queue_resolve_item) { .byte_idx = value >> 32, .byte_size = value & UINT32_MAX };
}

static bool x64_queue_resolve_item_eq(const def_data data_a, const def_data data_b) {
    (void) data_a;
    (void) data_b;
    return false;
}

static void x64_queue_resolve_item_print(const def_data data, FILE *file, int32_t idnt, uint32_t print_opts) {
    (void) print_opts;
    x64_queue_resolve_item item = x64_queue_resolve_item_decode(data);
    fprintf(file, "%*s", idnt, "");
    fprintf(file, "{ " COLOR(BOLD) "%d " COLOR(RESET), item.byte_idx);
    const char *queue_size_str = "INVALID QUEUE SIZE";
    switch (item.byte_size) {
        case X64_QUEUE_SIZE(8):
            queue_size_str = COLOR2(BOLD, YELLOW) "SIZE8" COLOR(RESET);
            break;
        case X64_QUEUE_SIZE(32):
            queue_size_str = COLOR2(BOLD, YELLOW) "SIZE32" COLOR(RESET);
            break;
        default:
            break;
    }
    fprintf(file, queue_size_str);
    fprintf(file, " }");
}

def_fn_table queue_resolve_fn_table = {
    .hash_fn = nullptr,
    .cmp_fn = nullptr,
    .eq_fn = x64_queue_resolve_item_eq,
    .copy_fn = nullptr,
    .serialize_fn = nullptr,
    .print_fn = x64_queue_resolve_item_print,
    .free_fn = nullptr
};

MEM_POOL(queue_item_pool);

static x64_queue_item *x64_queue_item_init(int32_t byte_idx, ssize_t label) {
    x64_queue_item *item = mem_alloc(&queue_item_pool, sizeof(x64_queue_item));
    item->byte_idx = byte_idx;
    item->label = label;
    item->resolves = list_init(0, &queue_resolve_fn_table);
    return item;
}

static void x64_queue_item_free(void *data) {
    x64_queue_item *item = data;
    list_free(item->resolves);
    mem_free(&queue_item_pool, item);
}

static void x64_queue_item_print(const def_data data, FILE *file, int32_t idnt, uint32_t print_opts) {
    const x64_queue_item *item = data.ptr;
    fprintf(file, "%*s", idnt, "");
    fprintf(file, COLOR2(BOLD, LIGHT_YELLOW) "%ld" COLOR(RESET) ":" COLOR(BOLD) "%d " COLOR(RESET) "->",
        item->label, item->byte_idx);
    list_print(item->resolves, file, 1, LIST_PRINT(_));
    if (print_opts & X64_QUEUE_ITEM_PRINT(NL_END))
        fprintf(file, "\n");
}

static size_t x64_queue_asm_item_hash(const def_data data) {
    const x64_queue_item *item = data.ptr;
    return item->label;
}

static bool x64_queue_asm_item_eq(const def_data data_a, const def_data data_b) {
    const x64_queue_item *item_a = data_a.ptr, *item_b = data_b.ptr;
    return item_a->label == item_b->label;
}

def_fn_table queue_asm_fn_table = {
    .hash_fn = x64_queue_asm_item_hash,
    .cmp_fn = nullptr,
    .eq_fn = x64_queue_asm_item_eq,
    .copy_fn = nullptr,
    .serialize_fn = nullptr,
    .print_fn = x64_queue_item_print,
    .free_fn = x64_queue_item_free
};

map *x64_queue_asm_init(void) {
    return map_init(0, X64_QUEUE_ITEM_PRINT(NL_END), &queue_asm_fn_table);
}

static size_t x64_queue_dis_item_hash(const def_data data) {
    const x64_queue_item *item = data.ptr;
    return item->byte_idx;
}

static bool x64_queue_dis_item_eq(const def_data data_a, const def_data data_b) {
    const x64_queue_item *item_a = data_a.ptr, *item_b = data_b.ptr;
    return item_a->byte_idx == item_b->byte_idx;
}

def_fn_table queue_dis_fn_table = {
    .hash_fn = x64_queue_dis_item_hash,
    .cmp_fn = nullptr,
    .eq_fn = x64_queue_dis_item_eq,
    .copy_fn = nullptr,
    .serialize_fn = nullptr,
    .print_fn = x64_queue_item_print,
    .free_fn = x64_queue_item_free
};

map *x64_queue_dis_init(void) {
    return map_init(0, X64_QUEUE_ITEM_PRINT(NL_END), &queue_dis_fn_table);
}

def_status x64_queue_add(map **queue, int32_t byte_idx, ssize_t label, int32_t resolve_byte_idx,
    x64_queue_size byte_size) {
    x64_queue_item find = { .byte_idx = byte_idx, .label = label };
    def_data found;
    if (map_action(queue, MAP_MODE(FIND), DEF_PTR(&find), &found) == DEF_STATUS(OK)) {
        x64_queue_item *item = found.ptr;
        if (byte_idx != -1)
            item->byte_idx = byte_idx;
        if (resolve_byte_idx != -1)
            list_add_back(item->resolves, x64_queue_resolve_item_encode(resolve_byte_idx, byte_size));
        return DEF_STATUS(OK);
    }
    x64_queue_item *item = x64_queue_item_init(byte_idx, label);
    if (map_action(queue, MAP_MODE(INSERT), DEF_PTR(item), &def_unused) != DEF_STATUS(OK)) {
        x64_queue_item_free(item);
        return DEF_STATUS(ERROR);
    }
    if (resolve_byte_idx != -1)
        list_add_back(item->resolves, x64_queue_resolve_item_encode(resolve_byte_idx, byte_size));
    return DEF_STATUS(OK);
}
