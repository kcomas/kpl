
#pragma once

#include "../header.h"

typedef struct _type_table {
    MEM_HEADER(_type_table);
    int32_t fn_idx; // -1 not used
    type *inner_type;
    map *type_map;
} type_table;

type_table *type_table_init(type *inner_type);

void type_table_free(type_table *table);

def_status type_table_add(type_table *table, type *inner_type);

size_t type_table_hash(const type_table *table);

bool type_table_eq(const type_table *table_a, const type_table *table_b);

void type_table_print(const type_table *table, FILE *file, int32_t idnt, type_print_opts print_opts);
