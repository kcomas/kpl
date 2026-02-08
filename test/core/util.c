
#include "../test.h"

TEST(core_absolute_file_path) {
    string *path_a = core_util_file_abs_path(nullptr, "./src/README.md");
    ASSERT(path_a && path_a->data, "invalid core_util_file_abs_path");
    string_print(path_a, stdout, 0, STRING_PRINT(NL_END));
    string *path_b = core_util_file_abs_path(path_a->data, "../README.md");
    ASSERT(path_b && path_b->data, "invalid core_util_file_abs_path");
    string_print(path_b, stdout, 0, STRING_PRINT(NL_END));
    ASSERT(!string_eq(path_a, path_b), "should not be equal");
    string_free(path_a);
    string_free(path_b);
}

static const char fib_import_file[] = {
#embed "../../example/fib_import.kpl"
, '\0' };

TEST(core_read_file_string) {
    string *fib = core_util_file_read_string("./example/fib_import.kpl");
    ASSERT(fib, "invalid file read");
    ASSERT(fib->len == strlen(fib_import_file), "invalid file len");
    string_free(fib);
}
