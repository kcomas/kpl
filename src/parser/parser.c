
#include "./parser.h"

error *parser_run(ast_container *cont, const string *str, type_base *base, ast_node *wrapper) {
    ast_container_init(cont, str, base, wrapper);
    type *lambda = namespace_table(TYPE_NAME(LAMBDA), nullptr, type_copy(namespace_type_void), nullptr);
    cont->root = ast_node_init(lambda, nullptr, ast_node_children_empty(),
            ast_position_init(cont->state.str_pos, 0, cont->state.line_no));
    ast_container_set_root_parent(cont, wrapper);
    return parser_next_list(cont, cont->root, lambda->class_union.table, PARSER_MODE(END));
}
