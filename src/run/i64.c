
#include "./i64.h"

size_t i64_hash(const def_data data) {
    return def_hash64shift(data.u64);
}

bool i64_eq(const def_data data_a, const def_data data_b) {
    return data_a.i64 == data_b.i64;
}

void i64_print(const def_data data, FILE *file, int32_t idnt, uint32_t opts) {
    fprintf(file, "%*s", idnt, "");
    printf(COLOR(MAGENTA) "%ld" COLOR(RESET), data.i64);
    if (opts & I64_PRINT(NL_END))
        fprintf(file, "\n");
}
