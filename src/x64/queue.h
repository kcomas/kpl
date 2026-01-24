
#pragma once

#include "./mem.h"

#define X64_QUEUE_SIZE(NAME) X64_QUEUE_SIZE_##NAME

typedef enum [[gnu::packed]] {
    X64_QUEUE_SIZE(8)   = 1,
    X64_QUEUE_SIZE(32)  = 4,
    X64_QUEUE_SIZE(_)   = 0
} x64_queue_size;

x64_queue_size x64_op_to_queue_size(x64_op_reg op);

typedef struct {
    int32_t byte_idx;
    x64_queue_size byte_size;
} x64_queue_resolve_item;

def_data x64_queue_resolve_item_encode(int32_t byte_idx, x64_queue_size byte_size);

x64_queue_resolve_item x64_queue_resolve_item_decode(def_data data);

typedef struct _x64_queue_item {
    MEM_HEADER(_x64_queue_item);
    int32_t byte_idx; // -1 not set
    ssize_t label; // -1 not set
    list *resolves;
} x64_queue_item;

#define X64_QUEUE_ITEM_PRINT(NAME) X64_QUEUE_ITEM_PRINT_##NAME

typedef enum [[gnu::packed]] {
    X64_QUEUE_ITEM_PRINT(NL_END)    = 1 << 0,
    X64_QUEUE_ITEM_PRINT(_)         = 0
} x64_queue_item_print_opts;

map *x64_queue_asm_init(void);

map *x64_queue_dis_init(void);

def_status x64_queue_add(map **queue, int32_t byte_idx, ssize_t label, int32_t resolve_byte_idx,
    x64_queue_size byte_size);
