
#pragma once

#include "./class/tag.h"
#include "./qualifier_flags.h"
#include "./class_name.h"

typedef union {
    type *inner_type;
    type_tag *tag;
} type_class_union;

typedef struct _type {
    MEM_HEADER(_type);
    type_class_union class_union;
    int32_t ref_count;
    type_qualifier_flags qualifier_flags;
    type_name name;
} type;
