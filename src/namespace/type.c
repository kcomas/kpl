
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
        list_add_back(li, def_ptr(ty));
    va_end(args);
    return type_init(name, TYPE_QUALIFIER_FLAG(_), type_class_union_list(li));
}

extern inline type *namespace_op(type_op_name op_name, type *return_type, type *left_type, type *right_type);

extern inline type *namespace_command(type_op_name op_name, type *return_type, type *left_type);

type *namespace_table(type_name name, type_table *parent_scope, type *inner_type, ...) {
    type_table *table = type_table_init(inner_type, parent_scope);
    va_list args;
    va_start(args, inner_type);
    type *ty;
    while ((ty = va_arg(args, type*)))
        if (type_table_add(table, ty) != DEF_STATUS(OK)) {
            va_end(args);
            type_table_free(table);
            return nullptr;
        }
    va_end(args);
    return type_init(name, TYPE_QUALIFIER_FLAG(_), type_class_union_table(table));
}

extern inline type *namespace_tag(type_name name, type *inner_type, const char *symbol, uint32_t print_opts,
    def_fn_table *fn_table, def_data data);

extern inline type *namespace_value(type *inner_type, uint32_t print_opts, def_fn_table *fn_table, def_data data);

extern inline type *namespace_qualifier(type *ty, type_qualifier_flags qualifier);

type *namespace_type_void = nullptr;

type *namespace_type_const_bool = nullptr;

[[gnu::constructor(DEF_CONSTRUCTOR_TYPE)]] static void namepsace_type_constructor(void) {
    namespace_type_void = type_init(TYPE_NAME(VOID), TYPE_QUALIFIER_FLAG(_), type_class_union_empty());
    namespace_type_const_bool = type_init(TYPE_NAME(BOOL), TYPE_QUALIFIER(CONST), type_class_union_empty());
}

[[gnu::destructor(DEF_DESTRUCTOR_TYPE)]] static void namespace_type_destructor(void) {
    type_free(namespace_type_void);
    type_free(namespace_type_const_bool);
}
