
#pragma once

#include "../run/run.h"
#include "./qualifier_flags.h"
#include "./class_name.h"

typedef union {
    // TODO 24 byte max
} type_class_data;

typedef struct _type {
    MEM_HEADER(_type);
    type_class_data class_data;
    uint32_t ref_count;
    type_qualifier_flags qualifier_flags;
    type_name name;
} type;
