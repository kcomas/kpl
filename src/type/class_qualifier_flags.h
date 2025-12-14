
#pragma once

#define TYPE_CLASS(NAME) TYPE_CLASS_##NAME

typedef enum [[gnu::packed]] {
    TYPE_CLASS(SCALAR),
    TYPE_CLASS(ARRAY),
    TYPE_CLASS(VECOR),
    TYPE_CLASS(OP),
    TYPE_CLASS(ENUM),
    TYPE_CLASS(STRUCT),
    TYPE_CLASS(FN),
    TYPE_CLASS(LAMBDA),
    TYPE_CLASS(OVERLOAD),
    TYPE_CLASS(TAG),
    TYPE_CLASS(ALIAS),
    TYPE_CLASS(VAR),
    TYPE_CLASS(_)
} type_class;

const char *type_class_str(type_class class);

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

#define TYPE_FLAG_MASK (TYPE_MASK(MOVED) | TYPE_FLAG(TRACING) | TYPE_FLAG(STACK))

#define TYPE_FLAG_TRACING TYPE_FLAG(A)

#define TYPE_FLAG_VAR_MOVED TYPE_FLAG(A)

#define TYPE_FLAG_VAR_ARG TYPE_FLAG(B)

#define TYPE_FLAG_VAR_REGISTER TYPE_FLAG(C)

#define TYPE_FLAG_FN_ADDRESS TYPE_FLAG(A)
