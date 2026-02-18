
#include "./f64.h"

size_t f64_hash(const def_data data) {
    return def_hash64shift(data.u64);
}

bool f64_eq(const def_data data_a, const def_data data_b) {
    return data_a.f64 == data_b.f64;
}

def_data f64_copy_fn(const def_data data) {
    return data;
}

ser_buffer *f64_serialize_fn(const def_data data) {
    ser_buffer *buffer = ser_buffer_init(sizeof(double));
    memcpy(buffer->data, &data.f64, sizeof(double));
    return buffer;
}

void f64_print(const def_data data, FILE *file, int32_t idnt, uint32_t print_opts) {
    fprintf(file, "%*s", idnt, "");
    fprintf(file, COLOR(YELLOW) "%lf" COLOR(RESET), data.f64);
    if (print_opts & F64_PRINT(NL_END))
        fprintf(file, "\n");
}

def_fn_table f64_fn_table = {
    .hash_fn = f64_hash,
    .cmp_fn = nullptr,
    .eq_fn = f64_eq,
    .copy_fn = f64_copy_fn,
    .serialize_fn = f64_serialize_fn,
    .print_fn = f64_print,
    .free_fn = nullptr
};
