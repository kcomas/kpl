
#pragma once

#include "./class/tag.h"
#include "./class/fixed.h"
#include "./class/list.h"
#include "./class/table.h"

typedef union {
    type *inner_type;
    type_fixed *fixed;
    list *li;
    type_table *table;
    type_tag *tag;
} type_class_union;

#define TYPE_CLASS_UNION_EMPTY (type_class_union) { .inner_type = NULL }

#define TYPE_CLASS_UNION_INNER(TYPE) (type_class_union) { .inner_type = TYPE }

#define TYPE_CLASS_UNION_TAG(TAG) (type_class_union) { .tag = TAG }

typedef struct _type {
    MEM_HEADER(_type);
    uint16_t ref_count;
    type_name name;
    type_qualifier_flags qualifier_flags;
    type_class_union class_union;
} type;

type *type_init(type_name name, type_qualifier_flags qualifier_flags, type_class_union class_union);

// type_free, type_hash, type_eq, type_copy, type_print in ./header.h
