
#pragma once

#include "./header.h"

typedef struct _list_item {
    MEM_HEADER(_list_item);
    def_data data;
} list_item;

typedef struct _list {
    RUN_HEADER(_list);
    list_item *head, *tail;
} list;

list *list_init(uint32_t print_opts, def_fn_table *fn_table);

void list_free(list *li);

void list_add_back(list *li, def_data data);

size_t list_hash(const list *li);

bool list_eq(const list *li_a, const list *li_b);

#define LIST_PRINT(NAME) LIST_PRINT_##NAME

typedef enum [[gnu::packed]] {
    LIST_PRINT(NL_END)          = 1 << 0,
    LIST_PRINT(NO_FIRST_IDNT)   = 1 << 1,
    LIST_PRINT(SEMI_SPACER)     = 1 << 2,
    LIST_PRINT(_)               = 0
} list_print_opts;

void list_print(const list *li, FILE *file, int32_t idnt, list_print_opts print_opts);
