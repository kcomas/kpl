
#include "./type.h"

extern inline type *namespace_scalar(type_name name);

extern inline type *namespace_fixed(type_name name, type *inner_type, uint32_t value);

extern inline type *namespace_vector(type_name name, type *inner_type);

type *namespace_list(type_name name, ...) {
    list *li = type_list_init();
    va_list args;
    va_start(args, name);
    type *ty;
    while ((ty = va_arg(args, type*)))
        list_add_back(li, DEF_PTR(ty));
    va_end(args);
    return type_init(name, TYPE_QUALIFIER_FLAG(_), TYPE_CLASS_UNION_LIST(li));
}

type *namespace_table(type_name name, type *inner_type, ...) {
    type_table *table = type_table_init(inner_type);
    va_list args;
    va_start(args, inner_type);
    type *ty;
    while ((ty = va_arg(args, type*)))
        if (type_table_add(table, ty) != DEF_STATUS(OK)) {
            va_end(args);
            type_table_free(table);
            return NULL;
        }
    va_end(args);
    return type_init(name, TYPE_QUALIFIER_FLAG(_), TYPE_CLASS_UNION_TABLE(table));
}

extern inline type *namespace_tag(type_name name, type *inner_type, const char *symbol, uint32_t print_opts,
    def_fn_table *fn_table, def_data data);

extern inline type *namespace_qualifier(type *ty, type_qualifier_flags qualifier);
