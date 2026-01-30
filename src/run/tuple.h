
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

#define TUPLE_PRINT_OPTS(NAME) TUPLE_PRINT_OPTS_##NAME

typedef enum [[gnu::packed]] {
    TUPLE_PRINT_OPTS(NL_END)        = 1 << 0,
    TUPLE_PRINT_OPTS(NL_EACH)       = 1 << 1,
    TUPLE_PRINT_OPTS(NO_FIRST_IDNT) = 1 << 2,
    TUPLE_PRINT_OPTS(_)             = 0
} tuple_print_opts;

void tuple_print(const tuple *tu, FILE *file, uint32_t idnt, tuple_print_opts print_opts);
