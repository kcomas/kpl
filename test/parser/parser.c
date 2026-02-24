
#include "./helper.h"

TEST(parser_simple_stmt) {
    const char *c_str = "1 + -2 * 3";
    const type_name type_check_array[] = {
        LIST(LAMBDA, NONE, OP(ATOM(VALUE), OP(NONE, OP(ATOM(VALUE), ATOM(VALUE)))))
    };
    ASSERT(parser_test(c_str, type_check_array), "invalid ast");
}

TEST(parser_cmd_fn) {
    const char *c_str = "log_add1 : Fn[I64] { I64`x; x\\log; x + 1 }";
    const type_name type_check_array[] = {
        LIST(LAMBDA, NONE, OP(ATOM(VAR),
            LIST(LAMBDA, LIST(DEFINE, LIST(FN, NONE, NONE), ATOM(I64)),
                LEFT(TAG, ATOM(I64)),
                LEFT(COMMAND, ATOM(VAR)),
                OP(ATOM(VAR), ATOM(VALUE)))))
    };
    ASSERT(parser_test(c_str, type_check_array), "invalid ast");
}

TEST(parser_base) {
    const char *c_str = ".io`print(\"Hello\\n\")";
    const type_name type_check_array[] = {
        LIST(LAMBDA, NONE, LIST(APPLY, LEFT(TAG, OP(NONE, NONE)), ATOM(VALUE)))
    };
    ASSERT(parser_test(c_str, type_check_array), "invalid ast");
}
