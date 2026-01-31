
#include "./def.h"

extern inline def_data def(void);

extern inline def_data def_b(bool b);

extern inline def_data def_u8(uint8_t u8);

extern inline def_data def_u16(uint16_t u16);

extern inline def_data def_u32(uint32_t u32);

extern inline def_data def_u64(uint64_t u64);

extern inline def_data def_i8(int8_t i8);

extern inline def_data def_i16(int16_t i16);

extern inline def_data def_i32(int32_t i32);

extern inline def_data def_i64(int64_t i64);

extern inline def_data def_f32(float f32);

extern inline def_data def_f64(double f64);

extern inline def_data def_ptr(void *ptr);

def_data def_unused = (def_data) { .ptr = nullptr };

static size_t def_unused_hash_fn(const def_data data) {
    return (size_t) data.u64;
}

static bool def_unused_eq_fn(const def_data data_a, const def_data data_b) {
    return data_a.u64 == data_b.u64;
}

static void def_unused_print_fn(const def_data data, FILE *file, int32_t idnt, uint32_t print_opts) {
    (void) data;
    (void) file;
    (void) idnt;
    (void) print_opts;
}

def_fn_table def_unused_fn_table = {
    .hash_fn = def_unused_hash_fn,
    .cmp_fn = nullptr,
    .eq_fn = def_unused_eq_fn,
    .copy_fn = nullptr,
    .serialize_fn = nullptr,
    .print_fn = def_unused_print_fn,
    .free_fn = nullptr
};

size_t def_hash64shift(size_t key) {
    key = (~key) + (key << 21);
    key ^= key >> 24;
    key += (key << 3) + (key << 8);
    key ^= key >> 14;
    key += (key << 2) + (key + 4);
    key ^= (key >> 28);
    key += (key << 31);
    return key;
}

void def_mask_print(uint64_t mask, int32_t max_bit, const char *color_str, def_bit_str_fn str_fn, FILE *file) {
    bool print_prev = false;
    for (int32_t bit_idx = 0; bit_idx <= max_bit; bit_idx++) {
        if (!(mask & (1L << bit_idx)))
            continue;
        if (print_prev)
            fprintf(file, "| ");
        print_prev = true;
        fprintf(file, "%s%s " COLOR(RESET), color_str, str_fn(bit_idx));
    }
}
