
#include "../test.h"

TEST(namespace_absolute_file_path) {
    string *path_a = namespace_util_file_abs_path(NULL, "./test.h");
    string *path_b = namespace_util_file_abs_path(NULL, "../test.h");
    ASSERT(path_a && path_b, "invalid namespace_util_file_abs_path");
    string_print(path_a, stdout, 0, STRING_PRINT(NL_END));
    string_print(path_b, stdout, 0, STRING_PRINT(NL_END));
    ASSERT(!string_eq(path_a, path_b), "should not be equal");
    string_free(path_a);
    string_free(path_b);
}
