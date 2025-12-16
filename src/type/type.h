
#pragma once

#include "./class/tag.h"

typedef union {
    type *inner_type;
    type_tag *tag;
} type_class_union;

typedef struct _type {
    MEM_HEADER(_type);
    int16_t ref_count;
    type_name name;
    type_qualifier_flags qualifier_flags;
    type_class_union class_union;
} type;

type *type_init(type_name name, type_qualifier_flags qualifier_flags, type_class_union class_union);

// type_free in ./header.h
