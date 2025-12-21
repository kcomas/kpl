
#pragma once

#include "./util.h"

#define QUEUE_ITEM_STATE(NAME) QUEUE_ITEM_STATE_##NAME

typedef enum [[gnu::packed]] {
    QUEUE_ITEM_STATE(INIT),
    QUEUE_ITEM_STATE(DEPS_FOUND),
    QUEUE_ITEM_STATE(RUNNING),
    QUEUE_ITEM_STATE(DONE)
} queue_item_state;

typedef struct _queue_item {
    MEM_HEADER(_queue_item);
    uint16_t dependencies;
    queue_item_state state;
    list *parents; // weak ref queue item
    string *filename, *filedata;
} queue_item;

#define QUEUE_ITEM_PRINT(NAME) QUEUE_ITEM_PRINT_##NAME

typedef enum [[gnu::packed]] {
    QUEUE_ITEM_PRINT(NL_END)            = 1 << 0,
    QUEUE_ITEM_PRINT(FILEDATA)          = 1 << 1,
    QUEUE_ITEM_PRINT(DEPENDENCIES)      = 1 << 2,
    QUEUE_ITEM_PRINT(_)                 = 0
} queue_item_print_opts;

void queue_item_add_parent(queue_item *restrict dependent, queue_item *restrict parent);

map *queue_init(queue_item_print_opts opts);

queue_item *queue_add(map **queue, const char *resolvepath, const char *filepath);
