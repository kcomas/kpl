
#include "./init.h"

ast_node *parser_ast_node_init_type(const token_slice *slice) {
    type *ty = parser_type(slice);
    if (!ty)
        return nullptr;
    return ast_node_init_slice(ty, nullptr, slice);
}

ast_node *parser_ast_node_init_var(const token_slice *slice) {
    type_tag *tag = type_tag_init(nullptr, 0, &def_unused_fn_table, def());
    if (type_tag_symbol_from_c_str_slice(tag, slice->str->data + slice->str_start,
                slice->str_end - slice->str_start) != DEF_STATUS(OK)) {
        type_tag_free(tag);
        return nullptr;
    }
    return ast_node_init_slice(type_init(TYPE_NAME(VAR), TYPE_QUALIFIER_FLAG(_), type_class_union_tag(tag)),
            nullptr, slice);
}

ast_node *parser_ast_node_init_var_unused(const token_slice *slice) {
    ast_node *node = parser_ast_node_init_var(slice);
    if (!node)
        return nullptr;
    node->ty->qualifier_flags |= TYPE_FLAG(UNUSED);
    return node;
}

static ast_node *parser_ast_node_init_value(const token_slice *slice, type *inner_type,
        def_fn_table *fn_table, def_data data) {
    return ast_node_init_slice(namespace_value(inner_type, 0, fn_table, data), nullptr, slice);
}

ast_node *parser_ast_node_init_true_value(const token_slice *slice) {
    return parser_ast_node_init_value(slice, namespace_scalar(TYPE_NAME(BOOL)),
            &bool_fn_table, def_b(true));
}

ast_node *parser_ast_node_init_true_const(const token_slice *slice) {
    return parser_ast_node_init_value(slice, type_copy(namespace_type_const_bool),
            &bool_fn_table, def_b(true));
}

ast_node *parser_ast_node_init_false_value(const token_slice *slice) {
    return parser_ast_node_init_value(slice, namespace_scalar(TYPE_NAME(BOOL)),
            &bool_fn_table, def_b(false));
}

ast_node *parser_ast_node_init_false_const(const token_slice *slice) {
    return parser_ast_node_init_value(slice, type_copy(namespace_type_const_bool),
            &bool_fn_table, def_b(false));
}

static constexpr uint64_t max_integer = UINT64_MAX / 10;

ast_node *parser_ast_node_init_integer(const token_slice *slice) {
    uint64_t value = 0;
    for (uint32_t str_idx = slice->str_start; str_idx < slice->str_end; str_idx++) {
        uint64_t next_value = slice->str->data[str_idx] - '0';
        if (value > max_integer - next_value)
            return nullptr;
        value = value * 10 + next_value;
    }
    type_name name = TYPE_NAME(U64);
    def_fn_table *fn_table = &u64_fn_table;
    if (value <= INT64_MAX) {
        name = TYPE_NAME(I64);
        fn_table = &i64_fn_table;
    }
    return parser_ast_node_init_value(slice, namespace_scalar(name), fn_table, def_u64(value));
}

ast_node *parser_ast_node_init_float(const token_slice *slice) {
    const char *float_start = &slice->str->data[slice->str_start];
    char *float_end = (char*) &slice->str->data[slice->str_end];
    double value = strtod(float_start, &float_end);
    if (value == HUGE_VAL || value == -HUGE_VAL)
        return nullptr;
    return parser_ast_node_init_value(slice, namespace_scalar(TYPE_NAME(F64)), &f64_fn_table, def_f64(value));
}

ast_node *parser_ast_node_init_string(const token_slice *slice) {
    string *value = string_init_c_str_slice(slice->str->data + slice->str_start,
            slice->str_end - slice->str_start);
    return parser_ast_node_init_value(slice, namespace_scalar(TYPE_NAME(STRING)),
            &string_fn_table, def_ptr(value));
}

ast_node *parser_ast_node_init_symbol(const token_slice *slice) {
    type_tag *tag = type_tag_init(nullptr, 0, &def_unused_fn_table, def());
    if (type_tag_symbol_from_c_str_slice(tag, slice->str->data + slice->str_start + sizeof(char),
            slice->str_end - slice->str_start) != DEF_STATUS(OK)) {
        type_tag_free(tag);
        return nullptr;
    }
    return ast_node_init_slice(type_init(TYPE_NAME(TAG), TYPE_QUALIFIER_FLAG(_), type_class_union_tag(tag)),
            nullptr, slice);
}

ast_node *parser_ast_node_init_symbol_unused(const token_slice *slice) {
    ast_node *node = parser_ast_node_init_symbol(slice);
    if (!node)
        return nullptr;
    node->ty->qualifier_flags |= TYPE_FLAG(UNUSED);
    return node;
}

ast_node *parser_ast_node_init_command(const token_slice *slice) {
    type *ty = parser_command(slice);
    if (!ty)
        return nullptr;
    return ast_node_init_slice(ty, nullptr, slice);
}

ast_node *parser_ast_node_init_op(const token_slice *slice) {
    type *ty = parser_op(slice);
    if (!ty)
        return nullptr;
    return ast_node_init_slice(ty, nullptr, slice);
}
