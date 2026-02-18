
#pragma once

#include "./header.h"

size_t bool_hash(const def_data data);

bool bool_eq(const def_data data_a, const def_data data_b);

def_data bool_copy_fn(const def_data data);

ser_buffer *bool_serialize_fn(const def_data data);

#define BOOL_PRINT(NAME) BOOL_PRINT_##NAME

typedef enum [[gnu::packed]] {
    BOOL_PRINT(NL_END)  = 1 << 0,
    BOOL_PRINT(_)       = 0
} bool_print_opts;

void bool_print(const def_data data, FILE *file, int32_t idnt, uint32_t print_opts);

extern def_fn_table bool_fn_table;
