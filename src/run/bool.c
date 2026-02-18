
#include "./bool.h"

size_t bool_hash(const def_data data) {
    return (data.u64 + 1) << 1;
}

bool bool_eq(const def_data data_a, const def_data data_b) {
    return data_a.b == data_b.b;
}

def_data bool_copy_fn(const def_data data) {
    return data;
}

ser_buffer *bool_serialize_fn(const def_data data) {
    ser_buffer *buffer = ser_buffer_init(sizeof(bool));
    buffer->data[0] = data.b;
    return buffer;
}

void bool_print(const def_data data, FILE *file, int32_t idnt, uint32_t print_opts) {
    fprintf(file, "%*s", idnt, "");
    fprintf(file, COLOR(CYAN) "%s" COLOR(RESET), data.b ? "true" : "false");
    if (print_opts & BOOL_PRINT(NL_END))
        fprintf(file, "\n");
}

def_fn_table bool_fn_table = {
    .hash_fn = bool_hash,
    .cmp_fn = nullptr,
    .eq_fn = bool_eq,
    .copy_fn = bool_copy_fn,
    .serialize_fn = bool_serialize_fn,
    .print_fn = bool_print,
    .free_fn = nullptr
};
