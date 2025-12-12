
#include "../test.h"

TEST(map_i64) {
    map *ma = map_init(0, i64_hash, i64_eq, i64_print, 0, NULL);
    for (size_t idx = 0; idx < 7; idx++)
        ASSERT(map_action(&ma, MAP_MODE(INSERT), DEF_I64(idx), &def_unused) == DEF_STATUS(OK),
            "invalid map insert");
    map_print(ma, stdout, 1, MAP_PRINT(NL_END));
    ASSERT(ma->size == MAP_MIN_SIZE, "map should not have resized");
    map_free(ma);
}
