
#include "../test.h"

TEST(string_from_c_str) {
    const char *example = "Hello World";
    string *st = string_init_c_str(example);
    string_print(st, stdout, 0, STRING_PRINT(NL_END));
    ASSERT(!strcmp(st->data, example), "invalid string init");
    string_free(st);
}
