
#pragma once

#include "./util.h"

typedef struct _core_queue core_queue;

#define CORE_QUEUE_ITEM_STATE(NAME) CORE_QUEUE_ITEM_STATE_##NAME

#define CORE_QUEUE_ITEM_FLAG(NAME) CORE_QUEUE_ITEM_FLAG_##NAME

#define CORE_QUEUE_ITEM_STATE_FLAG(NAME) CORE_QUEUE_ITEM_STATE_FLAG_##NAME

typedef enum [[gnu::packed]] {
    CORE_QUEUE_ITEM_STATE(INIT)             = 1 << 0,
    CORE_QUEUE_ITEM_STATE(DEPENDENCIES)     = 1 << 1,
    CORE_QUEUE_ITEM_STATE(RUNNING)          = 1 << 2,
    CORE_QUEUE_ITEM_STATE(DONE)             = 1 << 3,
    CORE_QUEUE_ITEM_FLAG(MAIN)              = 1 << 4,
    CORE_QUEUE_ITEM_STATE_FLAG(_)           = 0
} core_queue_item_state_flags;

#define CORE_QUEUE_ITEM_STATE_FLAGS_MAX_BIT 4

inline uint16_t core_queue_item_state_mask(void) {
    return CORE_QUEUE_ITEM_STATE(INIT) | CORE_QUEUE_ITEM_STATE(DEPENDENCIES) | \
        CORE_QUEUE_ITEM_STATE(RUNNING) | CORE_QUEUE_ITEM_STATE(DONE);
}

const char *core_queue_item_state_flags_str(int32_t bit_idx);

typedef struct _core_queue_item {
    MEM_HEADER(_core_queue_item);
    atomic_uint_least16_t dependencies;
    _Atomic core_queue_item_state_flags state_flags;
    list *parents; // weak ref queue item
    string *filename, *filedata;
} core_queue_item;

#define CORE_QUEUE_ITEM_PRINT(NAME) QUEUE_ITEM_PRINT_##NAME

typedef enum [[gnu::packed]] {
   CORE_QUEUE_ITEM_PRINT(NL_END)            = 1 << 0,
   CORE_QUEUE_ITEM_PRINT(FILEDATA)          = 1 << 1,
   CORE_QUEUE_ITEM_PRINT(DEPENDENCIES)      = 1 << 2,
   CORE_QUEUE_ITEM_PRINT(_)                 = 0
} core_queue_item_print_opts;

void core_queue_item_add_parent(core_queue_item *restrict dependent, core_queue_item *restrict parent);

extern def_fn_table core_queue_item_fn_table;

error *core_queue_item_error(core_queue_item *item, const char *msg);

typedef struct _core_queue {
    map *ma;
    error *er;
    pthread_mutex_t mutex;
    sem_t sem;
} core_queue;

void core_queue_init(core_queue *queue, core_queue_item_print_opts print_opts);

void core_queue_free(core_queue *queue);

// null for error
core_queue_item *core_queue_add(core_queue *queue, const char *resolvepath, const char *filepath);

void core_queue_error(core_queue *queue, error *er);
