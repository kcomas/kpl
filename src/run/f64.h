
#pragma once

#include "./header.h"

size_t f64_hash(const def_data data);

bool f64_eq(const def_data data_a, const def_data data_b);

def_data f64_copy_fn(const def_data data);

ser_buffer *f64_serialize_fn(const def_data data);

#define F64_PRINT(NAME) F64_PRINT_##NAME

typedef enum [[gnu::packed]] {
    F64_PRINT(NL_END)   = 1 << 0,
    F64_PRINT(_)        = 0
} f64_print_opts;

void f64_print(const def_data data, FILE *file, int32_t idnt, uint32_t print_opts);

extern def_fn_table f64_fn_table;
