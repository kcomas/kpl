
#include "./def.h"

def_data def_unused = DEF(_);

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
    .cmp_fn = NULL,
    .eq_fn = def_unused_eq_fn,
    .copy_fn = NULL,
    .serialize_fn = NULL,
    .print_fn = def_unused_print_fn,
    .free_fn = NULL
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
