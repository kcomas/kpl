
#pragma once

#include "./header.h"

typedef struct {
    uint32_t print_opts;
    def_fn_table *fn_table;
    def_data data;
} tuple_item;

typedef struct _tuple {
    MEM_HEADER(_tuple);
    uint32_t size;
    tuple_item items[];
} tuple;

tuple *tuple_init(uint32_t size);

void tuple_free(tuple *tu);

def_status tuple_set(tuple *tu, uint32_t print_opts, def_fn_table *fn_table, def_data data, uint32_t idx);

tuple_item *tuple_get(tuple *tu, uint32_t idx);

size_t tuple_hash(const tuple *tu);

bool tuple_eq(const tuple *tu_a, const tuple *tu_b);

#define TUPLE_PRINT(NAME) TUPLE_PRINT_OPTS_##NAME

typedef enum [[gnu::packed]] {
    TUPLE_PRINT(NL_END)        = 1 << 0,
    TUPLE_PRINT(NL_EACH)       = 1 << 1,
    TUPLE_PRINT(NO_FIRST_IDNT) = 1 << 2,
    TUPLE_PRINT(_)             = 0
} tuple_print_opts;

void tuple_print(const tuple *tu, FILE *file, uint32_t idnt, tuple_print_opts print_opts);
