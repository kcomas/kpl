
#pragma once

#include "./header.h"

size_t i64_hash(const def_data data);

bool i64_eq(const def_data data_a, const def_data data_b);

def_data i64_copy_fn(const def_data data);

ser_buffer *i64_serialize_fn(const def_data data);

#define I64_PRINT(NAME) I64_PRINT_##NAME

typedef enum [[gnu::packed]] {
    I64_PRINT(NL_END)   = 1 << 0
} i64_print_opts;

void i64_print(const def_data data, FILE *file, int32_t idnt, uint32_t print_opts);

extern def_fn_table i64_fn_table;
