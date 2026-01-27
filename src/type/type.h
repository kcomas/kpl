
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

#define TYPE_CLASS_UNION_EMPTY (type_class_union) { .inner_type = nullptr }

#define TYPE_CLASS_UNION_INNER(TYPE) (type_class_union) { .inner_type = TYPE }

#define TYPE_CLASS_UNION_FIXED(TYPE) (type_class_union) { .fixed = TYPE }

#define TYPE_CLASS_UNION_LIST(TYPE) (type_class_union) { .li = TYPE }

#define TYPE_CLASS_UNION_TABLE(TYPE) (type_class_union) { .table = TYPE }

#define TYPE_CLASS_UNION_TAG(TYPE) (type_class_union) { .tag = TYPE }

#define TYPE_CLASS_UNION_BASE(TYPE) (type_class_union) { .base = TYPE }

typedef struct _type {
    MEM_HEADER(_type);
    int32_t ref_count;
    type_class_union class_union;
    type_name name;
    type_qualifier_flags qualifier_flags;
} type;

type *type_init(type_name name, type_qualifier_flags qualifier_flags, type_class_union class_union);

// type_free, type_hash, type_eq, type_copy, type_print in ./header.h
