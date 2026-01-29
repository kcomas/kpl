
#pragma once

#include "./header.h"

size_t u64_hash(const def_data data);

bool u64_eq(const def_data data_a, const def_data data_b);

def_data u64_copy_fn(const def_data data);

ser_buffer *u64_serialize_fn(const def_data data);

#define U64_PRINT(NAME) U64_PRINT_##NAME

typedef enum [[gnu::packed]] {
    U64_PRINT(NL_END)   = 1 << 0,
    U64_PRINT(_)        = 0
} u64_print_opts;

void u64_print(const def_data data, FILE *file, int32_t idnt, uint32_t print_opts);

extern def_fn_table u64_fn_table;
