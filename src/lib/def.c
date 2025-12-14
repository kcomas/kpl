
#include "./def.h"

def_data def_unused = DEF(_);

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

def_fn_table def_unused_fn_table = {
    .hash_fn = NULL,
    .cmp_fn = NULL,
    .eq_fn = NULL,
    .copy_fn = NULL,
    .serialize_fn = NULL,
    .print_fn = NULL,
    .free_fn = NULL
};
