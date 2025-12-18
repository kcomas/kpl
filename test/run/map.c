
#include "../test.h"

#define MAP_I64_INSERT_COUNT 7

#define MAP_I64_UPSERT_COUNT 15

TEST(map_i64) {
    map *ma = map_init(0, 0, &i64_fn_table);
    for (size_t idx = 0; idx < MAP_I64_INSERT_COUNT; idx++)
        ASSERT(map_action(&ma, MAP_MODE(INSERT), DEF_I64(idx), &def_unused) == DEF_STATUS(OK),
            "invalid map insert");
    map_print(ma, stdout, 1, MAP_PRINT(NL_END) | MAP_PRINT(NO_FIRST_IDNT) | MAP_PRINT(SEMI_SPACER));
    ASSERT(ma->used == MAP_I64_INSERT_COUNT, "invalid insert count");
    ASSERT(ma->size == MAP_MIN_SIZE, "map should not have resized");
    ASSERT(ma->tail->data.i64 == MAP_I64_INSERT_COUNT - 1, "invalid tail value");
    for (size_t idx = 0; idx < MAP_I64_UPSERT_COUNT; idx++)
        ASSERT(map_action(&ma, MAP_MODE(UPSERT), DEF_I64(idx), &def_unused) == DEF_STATUS(OK),
            "invalid map upset");
    map_print(ma, stdout, 1, MAP_PRINT(NL_END) | MAP_PRINT(NO_FIRST_IDNT) | MAP_PRINT(SEMI_SPACER));
    ASSERT(ma->used == MAP_I64_UPSERT_COUNT, "invalid upsert count");
    ASSERT(ma->size == MAP_MIN_SIZE * MAP_RESIZE_MUL, "map should have resized");
    ASSERT(ma->tail->data.i64 == MAP_I64_UPSERT_COUNT - 1, "invalid tail value");
    map_free(ma);
}
