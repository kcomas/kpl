
#include "../test.h"

TEST(parser_simple_stmt) {
    ast_node wrapper;
    ast_container cont;
    string *str = string_init_c_str("1 + 2 * 3");
    //string *str = string_init_c_str("1 + 2");
    error *er = parser_run(&cont, str, nullptr, &wrapper);
    if (er) {
        error_print(er, stdout, 0, ERROR_PRINT(NL_END));
        error_free(er);
    }
    ASSERT(!er && cont.root, "invalid parser run");
    string_print(str, stdout, 0, STRING_PRINT(NL_END));
    ast_node_print(cont.root, stdout, 0, AST_NODE_PRINT(NL_END));
    ast_node_free(cont.root);
    string_free(str);
}
