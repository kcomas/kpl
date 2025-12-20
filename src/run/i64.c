
#include "./i64.h"

size_t i64_hash(const def_data data) {
    return def_hash64shift(data.u64);
}

bool i64_eq(const def_data data_a, const def_data data_b) {
    return data_a.i64 == data_b.i64;
}

def_data i64_copy_fn(const def_data data) {
    return data;
}

ser_buffer *i64_serialize_fn(const def_data data) {
    ser_buffer *buffer = ser_buffer_init(sizeof(int64_t));
    memcpy(buffer->data, &data.i64, sizeof(int64_t));
    return buffer;
}

void i64_print(const def_data data, FILE *file, int32_t idnt, uint32_t opts) {
    fprintf(file, "%*s", idnt, "");
    printf(COLOR(MAGENTA) "%ld" COLOR(RESET), data.i64);
    if (opts & I64_PRINT(NL_END))
        fprintf(file, "\n");
}

def_fn_table i64_fn_table = {
    .hash_fn = i64_hash,
    .cmp_fn = NULL,
    .eq_fn = i64_eq,
    .copy_fn = i64_copy_fn,
    .serialize_fn = i64_serialize_fn,
    .print_fn = i64_print,
    .free_fn = NULL
};
