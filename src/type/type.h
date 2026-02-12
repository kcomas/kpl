
#pragma once

#include "./class/tag.h"
#include "./class/fixed.h"
#include "./class/list.h"
#include "./class/table.h"
#include "./class/base.h"

typedef union {
    type *inner_type;
    type_fixed *fixed;
    list *li;
    type_table *table;
    type_tag *tag;
    type_base *base;
} type_class_union;

typedef struct _type {
    MEM_HEADER(_type);
    atomic_int_least32_t ref_count;
    type_class_union class_union;
    type_qualifier_flags qualifier_flags;
    type_op_name op_name;
    type_name name;
} type;

inline type_class_union type_class_union_empty(void) {
    return (type_class_union) { .inner_type = nullptr };
}

inline type_class_union type_class_union_inner(type *inner_type) {
    return (type_class_union) { .inner_type = inner_type };
}

inline type_class_union type_class_union_fixed(type_fixed *fixed) {
    return (type_class_union) { .fixed = fixed };
}

inline type_class_union type_class_union_list(list *li) {
    return (type_class_union) { .li = li };
}

inline type_class_union type_class_union_table(type_table *table) {
    return (type_class_union) { .table = table };
}

inline type_class_union type_class_union_tag(type_tag *tag) {
    return (type_class_union) { .tag = tag };
}

inline type_class_union type_class_union_base(type_base *base) {
    return (type_class_union) { .base = base };
}

type *type_init(type_name name, type_qualifier_flags qualifier_flags, type_class_union class_union);

// type_free, type_hash, type_eq, type_copy, type_print in ./header.h
