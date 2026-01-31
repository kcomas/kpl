
#include "../../test.h"

TEST(type_table) {
    type_table *table = type_table_init(type_init(TYPE_NAME(U64), TYPE_QUALIFIER_FLAG(_),
        type_class_union_empty()));
    type_tag *tag = type_tag_init(type_init(TYPE_NAME(U64), TYPE_QUALIFIER_FLAG(_), type_class_union_empty()),
        0, &def_unused_fn_table, def());
    const char *tag_name = "n";
    ASSERT(type_tag_symbol_from_c_str_slice(tag, tag_name, strlen(tag_name)) == DEF_STATUS(OK), "invalid symbol");
    ASSERT(type_table_add(table, type_init(TYPE_NAME(TAG), TYPE_QUALIFIER_FLAG(_), type_class_union_tag(tag))) ==
        DEF_STATUS(OK), "invalid table add");
    type_table_print(table, stdout, 0, TYPE_PRINT(CLASS_NL_END));
    type_table_free(table);
}
