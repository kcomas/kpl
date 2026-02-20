
#include "./helper.h"

TEST(parser_simple_stmt) {
    ast_node wrapper;
    ast_container cont;
    const type_name type_check_array[] = {
        LIST(LAMBDA, NONE, OP(ATOM(VALUE), OP(NONE, OP(ATOM(VALUE), ATOM(VALUE)))))
    };
    uint32_t type_check_array_idx = 0;
    string *str = string_init_c_str("1 + -2 * 3");
    error *er = parser_run(&cont, str, nullptr, &wrapper);
    if (er) {
        error_print(er, stdout, 0, ERROR_PRINT(NL_END));
        error_free(er);
    }
    ASSERT(!er && cont.root, "invalid parser run");
    string_print(str, stdout, 0, STRING_PRINT(NL_END));
    ast_node_print(cont.root, stdout, 0, AST_NODE_PRINT(NL_END));
    ASSERT(parser_check(cont.root, type_check_array, &type_check_array_idx), "invalid ast");
    ast_node_free(cont.root);
    string_free(str);
}

TEST(parser_cmd_fn) {
    ast_node wrapper;
    ast_container cont;
    const type_name type_check_array[] = {
        LIST(LAMBDA, NONE, OP(ATOM(VAR),
            LIST(LAMBDA, LIST(DEFINE, LIST(FN, NONE, NONE), ATOM(I64)),
                LEFT(TAG, ATOM(I64)),
                LEFT(COMMAND, ATOM(VAR)),
                OP(ATOM(VAR), ATOM(VALUE)))))
    };
    uint32_t type_check_array_idx = 0;
    string *str = string_init_c_str("log_add1 : Fn[I64] { I64`x; x\\log; x + 1 }");
    error *er = parser_run(&cont, str, nullptr, &wrapper);
    if (er) {
        error_print(er, stdout, 0, ERROR_PRINT(NL_END));
        error_free(er);
    }
    ASSERT(!er && cont.root, "invalid parser run");
    string_print(str, stdout, 0, STRING_PRINT(NL_END));
    ast_node_print(cont.root, stdout, 0, AST_NODE_PRINT(NL_END));
    ASSERT(parser_check(cont.root, type_check_array, &type_check_array_idx), "invalid ast");
    ast_node_free(cont.root);
    string_free(str);
}

TEST(parser_base) {
    ast_node wrapper;
    ast_container cont;
    const type_name type_check_array[] = {
        LIST(LAMBDA, NONE, LIST(APPLY, LEFT(TAG, OP(NONE, NONE)), ATOM(VALUE)))
    };
    uint32_t type_check_array_idx = 0;
    string *str = string_init_c_str(".io`print(\"Hello\\n\")");
    error *er = parser_run(&cont, str, nullptr, &wrapper);
    if (er) {
        error_print(er, stdout, 0, ERROR_PRINT(NL_END));
        error_free(er);
    }
    ASSERT(!er && cont.root, "invalid parser run");
    string_print(str, stdout, 0, STRING_PRINT(NL_END));
    ast_node_print(cont.root, stdout, 0, AST_NODE_PRINT(NL_END));
    ASSERT(parser_check(cont.root, type_check_array, &type_check_array_idx), "invalid ast");
    ast_node_free(cont.root);
    string_free(str);
}
