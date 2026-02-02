
#include "../test.h"

TEST(namespace_std_example) {
    for (int64_t idx = 1; idx <= 5; idx++) {
        const char symbol[] = { idx + '0', '\0' };
        type *value = namespace_tag(TYPE_NAME(VALUE), namespace_scalar(TYPE_NAME(I64)), symbol,
            I64_PRINT(_), &i64_fn_table, def_i64(idx));
        namespace_std_add(value, "example", nullptr);
    }
    type *string = namespace_tag(TYPE_NAME(VALUE), namespace_scalar(TYPE_NAME(STRING)), "greet",
        STRING_PRINT(_), &string_fn_table, def_ptr(string_init_c_str("Hello World")));
    namespace_std_add(string, "example", "strings", nullptr);
    namespace_std_print(stdout, 0, TYPE_PRINT(CLASS_NL_END));
}
