
#include "../../test.h"

TEST(type_list) {
    type *i64 = type_init(TYPE_NAME(I64), TYPE_QUALIFIER_FLAG(_), TYPE_CLASS_UNION_EMPTY);
    type_list *list = type_list_init();
    type_list_add(list, type_copy(i64));
    type_list_add(list, type_copy(i64));
    type_list_add(list, i64);
    type_list_print(list, stdout, 0, TYPE_PRINT(CLASS_NL_END));
    type_list_free(list);
}
