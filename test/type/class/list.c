
#include "../../test.h"

TEST(type_list) {
    type *i64 = type_init(TYPE_NAME(I64), TYPE_QUALIFIER_FLAG(_), TYPE_CLASS_UNION_EMPTY);
    list *li = type_list_init();
    type_list_add(li, type_copy(i64));
    type_list_add(li, type_copy(i64));
    type_list_add(li, i64);
    type_list_print(li, stdout, 0, TYPE_PRINT(CLASS_NL_END));
    type_list_free(li);
}
