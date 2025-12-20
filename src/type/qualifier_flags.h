
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
    TYPE_FLAG(A)                = 1 << 5,
    TYPE_FLAG(B)                = 1 << 6,
    TYPE_FLAG(C)                = 1 << 7,
    TYPE_QUALIFIER_FLAG(_)      = 0,
} type_qualifier_flags;

const char *type_qualifier_str(type_qualifier_flags qualifier_flags);

#define TYPE_QUALIFIER_MASK (TYPE_QUALIFIER(CONST) | TYPE_QUALIFIER(REF) | \
    TYPE_QUALIFIER(SHARED) | TYPE_QUALIFIER(NAMESPACE) | TYPE_QUALIFIER(MUTEX))

#define TYPE_FLAG_MASK (TYPE_FLAG(A) | TYPE_FLAG(B) | TYPE_FLAG(C))

#define TYPE_FLAG_TRACING_GC TYPE_FLAG(A)

#define TYPE_FLAG_VAR_MOVED TYPE_FLAG(A)

#define TYPE_FLAG_TMP_REGISTER TYPE_FLAG(B)

#define TYPE_FLAG_VAR_REGISTER TYPE_FLAG(B)

#define TYPE_FLAG_ARG_REGISTER TYPE_FLAG(B)

#define TYPE_FLAG_FN_ADDRESS TYPE_FLAG(A)

#define TYPE_FLAG_THREAD_JOINABLE TYPE_FLAG(A)
