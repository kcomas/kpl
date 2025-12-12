
#include "../test.h"

TEST(error_example) {
    error *er = ERROR_INIT(NULL, 0, NULL, DEF(_), "Test");
    error_print(er, stdout, 0, ERROR_PRINT(NL_END));
    ASSERT(!strcmp(er->msg, "Test"), "invalid error message");
    error_free(er);
}
