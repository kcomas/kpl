
#include "./u64.h"

size_t u64_hash(const def_data data) {
    return def_hash64shift(data.u64);
}

bool u64_eq(const def_data data_a, const def_data data_b) {
    return data_a.u64 == data_b.u64;
}

def_data u64_copy_fn(const def_data data) {
    return data;
}

ser_buffer *u64_serialize_fn(const def_data data) {
    ser_buffer *buffer = ser_buffer_init(sizeof(uint64_t));
    memcpy(buffer->data, &data.u64, sizeof(uint64_t));
    return buffer;
}

void u64_print(const def_data data, FILE *file, int32_t idnt, uint64_t print_opts) {
    fprintf(file, "%*s", idnt, "");
    printf(COLOR(YELLOW) "%lu" COLOR(RESET), data.u64);
    if (print_opts & U64_PRINT(NL_END))
        fprintf(file, "\n");
}

def_fn_table u64_fn_table = {
    .hash_fn = u64_hash,
    .cmp_fn = nullptr,
    .eq_fn = u64_eq,
    .copy_fn = u64_copy_fn,
    .serialize_fn = u64_serialize_fn,
    .print_fn = u64_print,
    .free_fn = nullptr
};
