
#include "../test.h"

TEST(namespace_import_example) {
    string *filename = string_init_c_str("/example/path/file.kpl");
    ASSERT(namespace_import_insert(filename), "import should not be null");
    ASSERT(namespace_import_get(filename), "get should not be null");
    namespace_import_print(stdout, 0, TYPE_PRINT(CLASS_NL_END));
    string_free(filename);
}
