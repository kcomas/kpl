
#include "../test.h"

TEST(query_node_print) {
    uint32_t type_mask = type_name_create_mask(TYPE_NAME(I8), TYPE_NAME(I16), TYPE_NAME(I32), TYPE_NAME(I64), TYPE_NAME(_));
    query_node *value = query_node_init(nullptr, TYPE_CLASS(SCALAR), type_mask, nullptr);
    query_node *right = query_node_init(nullptr, TYPE_CLASS(TAG),
            type_name_create_mask(TYPE_NAME(VALUE), TYPE_NAME(_)), value, nullptr);
    query_node *left = query_node_init(nullptr, TYPE_CLASS(SCALAR),
            type_name_create_mask(TYPE_NAME(F64), TYPE_NAME(_)), nullptr);
    query_node *op = query_node_init(nullptr, TYPE_CLASS(LIST),
            type_name_create_mask(TYPE_NAME(OP), TYPE_NAME(_)),
            query_node_init(nullptr, TYPE_CLASS(_), 0, nullptr), left, right, nullptr);
    type_op_set_mask(op->type_op_name_mask, TYPE_OP_NAME(CAST), TYPE_OP_NAME(_));
    query_node_print(op, stdout, 0, QUERY_NODE_PRINT(NL_END));
    query_node_free(op);
}
