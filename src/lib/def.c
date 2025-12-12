
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
