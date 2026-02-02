
#include "../test.h"

TEST(tuple_example) {
    tuple *tu = tuple_init(2);
    ASSERT(tuple_set(tu, I64_PRINT(_), &i64_fn_table, def_i64(1), 0) == DEF_STATUS(OK), "invalid tuple set");
    ASSERT(tuple_set(tu, U64_PRINT(_), &u64_fn_table, def_u64(2), 1) == DEF_STATUS(OK), "invalid tuple set");
    const tuple_item *item = tuple_get(tu, 1);
    ASSERT(item && item->data.u64 == 2, "invalid tuple value");
    ASSERT(!tuple_get(tu, 2), "should be null");
    tuple_print(tu, stdout, 1, TUPLE_PRINT(NO_FIRST_IDNT) | TUPLE_PRINT(NL_END));
    tuple_free(tu);
    tu = tuple_init(1);
    ASSERT(!tu, "should be null");
}
