
#pragma once

#include "./util.h"

#define QUEUE_ITEM_STATE(NAME) QUEUE_ITEM_STATE_##NAME

typedef enum [[gnu::packed]] {
    QUEUE_ITEM_STATE(INIT),
    QUEUE_ITEM_STATE(LOAD_DEPS),
    QUEUE_ITEM_STATE(READY),
    QUEUE_ITEM_STATE(RUNNING),
    QUEUE_ITEM_STATE(DONE)
} core_queue_item_state;

typedef struct _core_queue_item {
    MEM_HEADER(_core_queue_item);
    uint16_t dependencies;
    core_queue_item_state state;
    list *parents; // weak ref queue item
    string *filename, *filedata;
} core_queue_item;

#define QUEUE_ITEM_PRINT(NAME) QUEUE_ITEM_PRINT_##NAME

typedef enum [[gnu::packed]] {
    QUEUE_ITEM_PRINT(NL_END)            = 1 << 0,
    QUEUE_ITEM_PRINT(FILEDATA)          = 1 << 1,
    QUEUE_ITEM_PRINT(DEPENDENCIES)      = 1 << 2,
    QUEUE_ITEM_PRINT(_)                 = 0
} core_queue_item_print_opts;

void core_queue_item_add_parent(core_queue_item *restrict dependent, core_queue_item *restrict parent);

typedef struct {
    map *ma;
    mtx_t mutex;
} core_queue;

void core_queue_init(core_queue *queue, core_queue_item_print_opts opts);

void core_queue_free(core_queue *queue);

// null for error
core_queue_item *core_queue_add(core_queue *queue, const char *resolvepath, const char *filepath);
