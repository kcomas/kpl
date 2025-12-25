
#include "../../test.h"

TEST(type_base) {
    type_base *base = type_base_init(string_init_c_str("root"));
    const char *names[] = { "a", "b", "c" };
    for (int64_t name_idx = 0; name_idx < (int64_t) (sizeof(names) / sizeof(names[0])); name_idx++) {
        type_tag *tag = type_tag_init(type_init(TYPE_NAME(I64), TYPE_QUALIFIER_FLAG(_), TYPE_CLASS_UNION_EMPTY),
            0, &i64_fn_table, DEF_I64(name_idx));
        ASSERT(type_tag_symbol_from_c_str_slice(tag, names[name_idx], strlen(names[name_idx])) == DEF_STATUS(OK),
            "invalid symbol");
        // TODO
        ASSERT(type_base_add(base, type_init(TYPE_NAME(VAR), TYPE_QUALIFIER_FLAG(_),
            TYPE_CLASS_UNION_TAG(tag))) == DEF_STATUS(OK), "invalid base add");
    }
    type_base_print(base, stdout, 0, TYPE_PRINT(CLASS_NL_END));
    type_base_free(base);
}
