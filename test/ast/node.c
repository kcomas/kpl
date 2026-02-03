
#include "../test.h"

TEST(ast_node_index) {
    ast_node *left = ast_node_init(namespace_tag(TYPE_NAME(VAR),
                namespace_vector(TYPE_NAME(VECTOR), namespace_scalar(TYPE_NAME(I64))), "vec",
                0, &def_unused_fn_table, def()), nullptr, ast_node_children_empty(), -1, 0, 0);
    ast_node *right = ast_node_init(namespace_tag(TYPE_NAME(VALUE), namespace_scalar(TYPE_NAME(U64)), nullptr,
                U64_PRINT(_), &u64_fn_table, def_u64(1)), nullptr, ast_node_children_empty(), -1, 0, 0);
    ast_node *op = ast_node_init(namespace_op(TYPE_OP_NAME(INDEX),
            namespace_vector(TYPE_NAME(RESULT), namespace_scalar(TYPE_NAME(I64))), nullptr, nullptr), nullptr,
            ast_node_children_empty(), -1, 0, 0);
    ASSERT(ast_node_op_set_side(op, AST_NODE_OP_SIDE(LEFT), left) == DEF_STATUS(OK), "invalid ast node op set");
    ASSERT(ast_node_op_set_side(op, AST_NODE_OP_SIDE(RIGHT), right) == DEF_STATUS(OK), "invalid ast node op set");
    ast_node_print(op, stdout, 0, AST_NODE_PRINT(NL_END));
    ast_node_free(op);
}
