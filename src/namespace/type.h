
#pragma once

#include "../type/type.h"

inline type *namespace_scalar(type_name name) {
    return type_init(name, TYPE_QUALIFIER_FLAG(_), TYPE_CLASS_UNION_EMPTY);
}

inline type *namespace_fixed(type_name name, type *inner_type, uint32_t value) {
    type_fixed *fixed = type_fixed_init(inner_type, value);
    return type_init(name, TYPE_QUALIFIER_FLAG(_), TYPE_CLASS_UNION_FIXED(fixed));
}

inline type *namespace_vector(type_name name, type *inner_type) {
    return type_init(name, TYPE_QUALIFIER_FLAG(_), TYPE_CLASS_UNION_INNER(inner_type));
}

type *namespace_list(type_name name, ...);

type *namespace_table(type_name name, type *inner_type, ...);

inline type *namespace_tag(type_name name, type *inner_type, const char *symbol, uint32_t print_opts,
    def_fn_table *fn_table, def_data data) {
    type_tag *tag = type_tag_init(inner_type, print_opts, fn_table, data);
    if (type_tag_symbol_from_c_str_slice(tag, symbol, strlen(symbol)) != DEF_STATUS(OK)) {
        type_tag_free(tag);
        return NULL;
    }
    return type_init(name, TYPE_QUALIFIER_FLAG(_), TYPE_CLASS_UNION_TAG(tag));
}

inline type *namespace_qualifier(type *ty, type_qualifier_flags qualifier) {
    ty->qualifier_flags |= qualifier;
    return ty;
}
