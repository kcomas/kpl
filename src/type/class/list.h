
#pragma once

#include "../header.h"

typedef struct _type_list_item {
    MEM_HEADER(_type_list_item);
    type *inner_type;
} type_list_item;

typedef struct _type_list {
    MEM_HEADER(_type_list);
    uint32_t len;
    type_list_item *head, *tail;
} type_list;

type_list *type_list_init(void);

void type_list_free(type_list *list);

void type_list_add(type_list *list, type *inner_type);

size_t type_list_hash(const type_list *list);

bool type_list_eq(const type_list *list_a, const type_list *list_b);

void type_list_print(const type_list *list, FILE *file, uint32_t idnt, type_print_opts opts);
