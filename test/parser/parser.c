
#include "../test.h"

TEST(parser_stmt) {
    string *str = string_init_c_str("1+2*3");
    string_free(str);
}
