
#pragma once

#define TYPE_QUALIFIER(NAME) TYPE_QUALIFIER_##NAME

#define TYPE_FLAG(NAME) TYPE_FLAG_##NAME

#define TYPE_QUALIFIER_FLAG(NAME) TYPE_QUALIFIER_FLAG_##NAME

typedef enum [[gnu::packed]] {
    TYPE_QUALIFIER(CONST)       = 1 << 0,
    TYPE_QUALIFIER(REF)         = 1 << 1,
    TYPE_QUALIFIER(SHARED)      = 1 << 2,
    TYPE_QUALIFIER(NAMESPACE)   = 1 << 3,
    TYPE_QUALIFIER(MUTEX)       = 1 << 4,
    TYPE_FLAG(TRACING_GC)       = 1 << 5,
    TYPE_FLAG(VAR_VALUE)        = 1 << 6,
    TYPE_FLAG(VAR_MOVED)        = 1 << 7,
    TYPE_FLAG(VAR_ARG)          = 1 << 8,
    TYPE_FLAG(VAR_REGISTER)     = 1 << 9,
    TYPE_FLAG(FN_ADDRESS)       = 1 << 10,
    TYPE_QUALIFIER_FLAG(_)      = 0,
} type_qualifier_flags;

const char *type_qualifier_str(type_qualifier_flags qualifier_flags);

#define TYPE_QUALIFIER_MASK (TYPE_QUALIFIER(CONST) | TYPE_QUALIFIER(REF) | \
    TYPE_QUALIFIER(SHARED) | TYPE_QUALIFIER(NAMESPACE) | TYPE_QUALIFIER(MUTEX))

#define TYPE_FLAG_MASK (TYPE_FLAG(TRACING_GC) | TYPE_FLAG(VAR_VALUE) | TYPE_FLAG(VAR_MOVED) | \
    TYPE_FLAG(VAR_MOVED) | TYPE_FLAG(VAR_ARG) | TYPE_FLAG(VAR_REGISTER) | TYPE_FLAG(FN_ADDRESS))
