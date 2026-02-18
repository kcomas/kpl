
#pragma once

#include "../type/type.h"

inline type *namespace_scalar(type_name name) {
    return type_init(name, TYPE_QUALIFIER_FLAG(_), type_class_union_empty());
}

inline type *namespace_fixed(type_name name, type *inner_type, uint32_t value) {
    type_fixed *fixed = type_fixed_init(inner_type, value);
    return type_init(name, TYPE_QUALIFIER_FLAG(_), type_class_union_fixed(fixed));
}

inline type *namespace_vector(type_name name, type *inner_type) {
    return type_init(name, TYPE_QUALIFIER_FLAG(_), type_class_union_inner(inner_type));
}

type *namespace_list(type_name name, ...);

inline type *namespace_op(type_op_name op_name, type *return_type, type *left_type, type *right_type) {
    list *li = type_list_init();
    list_add_back(li, def_ptr(return_type));
    list_add_back(li, def_ptr(left_type));
    list_add_back(li, def_ptr(right_type));
    type *ty = type_init(TYPE_NAME(OP), TYPE_QUALIFIER_FLAG(_), type_class_union_list(li));
    ty->op_name = op_name;
    return ty;
}

type *namespace_table(type_name name, type_table *parent_scope, type *inner_type, ...);

inline type *namespace_tag(type_name name, type *inner_type, const char *symbol, uint32_t print_opts,
    def_fn_table *fn_table, def_data data) {
    type_tag *tag = type_tag_init(inner_type, print_opts, fn_table, data);
    if (symbol && type_tag_symbol_from_c_str_slice(tag, symbol, strlen(symbol)) != DEF_STATUS(OK)) {
        type_tag_free(tag);
        return nullptr;
    }
    return type_init(name, TYPE_QUALIFIER_FLAG(_), type_class_union_tag(tag));
}

inline type *namespace_value(type *inner_type, uint32_t print_opts, def_fn_table *fn_table, def_data data) {
    return namespace_tag(TYPE_NAME(VALUE), inner_type, nullptr, print_opts, fn_table, data);
}

inline type *namespace_qualifier(type *ty, type_qualifier_flags qualifier) {
    ty->qualifier_flags |= qualifier;
    return ty;
}

extern type *namespace_type_void;

extern type *namespace_type_const_bool;
