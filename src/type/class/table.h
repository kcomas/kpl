
#pragma once

#include "../header.h"
#include "./tag.h"

typedef struct _type_table {
    MEM_HEADER(_type_table);
    int32_t fn_idx; // -1 not used
    type *inner_type;
    map *tag_map;
} type_table;

type_table *type_table_init(type *inner_type);

void type_table_free(type_table *table);

void type_table_print(const type_table *table, FILE *file, int32_t idnt, type_print_opts opts);
