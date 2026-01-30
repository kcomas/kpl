
#pragma once

#include "../run/run.h"

#define TYPE_QUALIFIER(NAME) TYPE_QUALIFIER_##NAME

#define TYPE_FLAG(NAME) TYPE_FLAG_##NAME

#define TYPE_QUALIFIER_FLAG(NAME) TYPE_QUALIFIER_FLAG_##NAME

typedef enum [[gnu::packed]] {
    TYPE_QUALIFIER(CONST)       = 1 << 0,
    TYPE_QUALIFIER(REF)         = 1 << 1,
    TYPE_QUALIFIER(SHARED)      = 1 << 2,
    TYPE_QUALIFIER(MUTEX)       = 1 << 3,
    TYPE_FLAG(TRACING_GC)       = 1 << 4,
    TYPE_FLAG(MOVED)            = 1 << 5,
    TYPE_FLAG(REGISTER)         = 1 << 6,
    TYPE_FLAG(C_CODE)           = 1 << 7,
    TYPE_QUALIFIER_FLAG(_)      = 0,
} type_qualifier_flags;

inline uint32_t type_qualifier_mask(void) {
    return TYPE_QUALIFIER(CONST) | TYPE_QUALIFIER(REF) | TYPE_QUALIFIER(SHARED) | TYPE_QUALIFIER(MUTEX);
}

const char *type_qualifier_str(type_qualifier_flags qualifier_flags);

inline uint32_t type_flag_mask(void) {
    return TYPE_FLAG(TRACING_GC) | TYPE_FLAG(MOVED) | TYPE_FLAG(REGISTER) | TYPE_FLAG(C_CODE);
}

const char *type_flag_str(type_qualifier_flags qualifier_flags);
