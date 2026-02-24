
#include "./class_name.h"

static const char *class_str_array[] = {
    "SCALAR",
    "FIXED",
    "VECTOR",
    "LIST",
    "TABLE",
    "TAG",
    "BASE",
    "NONE"
};

const char *type_class_str(type_class class) {
    if (class > TYPE_CLASS(_))
        return "INVALID TYPE CLASS";
    return class_str_array[class];
}

static const char *name_str_array[] = {
    // SCALAR
    "VOID",
    "BOOL",
    "U8",
    "U16",
    "U32",
    "U64",
    "I8",
    "I16",
    "I32",
    "I64",
    "F32",
    "F64",
    "CHAR",
    "STRING",
    "ERROR",
    "BUFFER",
    "FD",
    "SOCKET",
    // FIXED
    "ARRAY",
    "TMP",
    // VECTOR
    "VECTOR",
    "SET",
    "RESULT",
    "OPTION",
    "THREAD",
    // LIST
    "OP",
    "COMMAND",
    "TUPLE",
    "MAP",
    "OVERLOAD",
    "CLOSURE",
    // TABLE
    "ENUM",
    "STRUCT",
    "UNION",
    "FN",
    "COROUTINE",
    "LAMBDA",
    "APPLY",
    "DEFINE",
    "REGEX",
    "RANGE",
    // TAG
    "TAG",
    "VALUE",
    "ALIAS",
    "VAR",
    "ARG",
    "NAME",
    // BASE
    "BASE",
    "_"
};

const char *type_name_str(type_name name) {
    if (name >= TYPE_NAME(_))
        return "INVALID TYPE NAME";
    return name_str_array[name];
}

#define CLASS_NAME(CLASS, NAME) [TYPE_NAME(NAME)] = TYPE_CLASS(CLASS)

static const type_class type_name_class_array[] = {
    CLASS_NAME(SCALAR, VOID),
    CLASS_NAME(SCALAR, BOOL),
    CLASS_NAME(SCALAR, U8),
    CLASS_NAME(SCALAR, U16),
    CLASS_NAME(SCALAR, U32),
    CLASS_NAME(SCALAR, U64),
    CLASS_NAME(SCALAR, I8),
    CLASS_NAME(SCALAR, I16),
    CLASS_NAME(SCALAR, I32),
    CLASS_NAME(SCALAR, I64),
    CLASS_NAME(SCALAR, F32),
    CLASS_NAME(SCALAR, F64),
    CLASS_NAME(SCALAR, CHAR),
    CLASS_NAME(SCALAR, STRING),
    CLASS_NAME(SCALAR, BUFFER),
    CLASS_NAME(SCALAR, ERROR),
    CLASS_NAME(SCALAR, FD),
    CLASS_NAME(SCALAR, SOCKET),
    CLASS_NAME(FIXED, ARRAY),
    CLASS_NAME(FIXED, TMP),
    CLASS_NAME(VECTOR, VECTOR),
    CLASS_NAME(VECTOR, SET),
    CLASS_NAME(VECTOR, RESULT),
    CLASS_NAME(VECTOR, OPTION),
    CLASS_NAME(VECTOR, THREAD),
    CLASS_NAME(LIST, OP),
    CLASS_NAME(LIST, COMMAND),
    CLASS_NAME(LIST, TUPLE),
    CLASS_NAME(LIST, MAP),
    CLASS_NAME(LIST, OVERLOAD),
    CLASS_NAME(LIST, CLOSURE),
    CLASS_NAME(TABLE, ENUM),
    CLASS_NAME(TABLE, STRUCT),
    CLASS_NAME(TABLE, UNION),
    CLASS_NAME(TABLE, FN),
    CLASS_NAME(TABLE, COROUTINE),
    CLASS_NAME(TABLE, LAMBDA),
    CLASS_NAME(TABLE, DEFINE),
    CLASS_NAME(TABLE, APPLY),
    CLASS_NAME(TABLE, REGEX),
    CLASS_NAME(TABLE, RANGE),
    CLASS_NAME(TAG, TAG),
    CLASS_NAME(TAG, VALUE),
    CLASS_NAME(TAG, ALIAS),
    CLASS_NAME(TAG, VAR),
    CLASS_NAME(TAG, ARG),
    CLASS_NAME(BASE, BASE),
    CLASS_NAME(_, _)
};

type_class type_name_get_class(type_name name) {
    if (name >= TYPE_NAME(_))
        return TYPE_CLASS(_);
    return type_name_class_array[name];
}

static const uint32_t type_name_mask_array[] = {
    // SCALAR
    [TYPE_NAME(VOID)]       = 1 << 0,
    [TYPE_NAME(BOOL)]       = 1 << 1,
    [TYPE_NAME(U8)]         = 1 << 2,
    [TYPE_NAME(U16)]        = 1 << 3,
    [TYPE_NAME(U32)]        = 1 << 4,
    [TYPE_NAME(U64)]        = 1 << 5,
    [TYPE_NAME(I8)]         = 1 << 6,
    [TYPE_NAME(I16)]        = 1 << 7,
    [TYPE_NAME(I32)]        = 1 << 8,
    [TYPE_NAME(I64)]        = 1 << 9,
    [TYPE_NAME(F32)]        = 1 << 10,
    [TYPE_NAME(F64)]        = 1 << 11,
    [TYPE_NAME(CHAR)]       = 1 << 12,
    [TYPE_NAME(STRING)]     = 1 << 13,
    [TYPE_NAME(ERROR)]      = 1 << 14,
    [TYPE_NAME(BUFFER)]     = 1 << 15,
    [TYPE_NAME(FD)]         = 1 << 16,
    [TYPE_NAME(SOCKET)]     = 1 << 17,
    // FIXED
    [TYPE_NAME(ARRAY)]      = 1 << 0,
    [TYPE_NAME(TMP)]        = 1 << 1,
    // VECTOR
    [TYPE_NAME(VECTOR)]     = 1 << 0,
    [TYPE_NAME(SET)]        = 1 << 1,
    [TYPE_NAME(RESULT)]     = 1 << 2,
    [TYPE_NAME(OPTION)]     = 1 << 3,
    [TYPE_NAME(THREAD)]     = 1 << 4,
    // LIST
    [TYPE_NAME(OP)]         = 1 << 0,
    [TYPE_NAME(COMMAND)]    = 1 << 1,
    [TYPE_NAME(TUPLE)]      = 1 << 2,
    [TYPE_NAME(MAP)]        = 1 << 3,
    [TYPE_NAME(OVERLOAD)]   = 1 << 4,
    [TYPE_NAME(CLOSURE)]    = 1 << 5,
    // TABLE
    [TYPE_NAME(ENUM)]       = 1 << 0,
    [TYPE_NAME(STRUCT)]     = 1 << 1,
    [TYPE_NAME(UNION)]      = 1 << 2,
    [TYPE_NAME(FN)]         = 1 << 3,
    [TYPE_NAME(COROUTINE)]  = 1 << 4,
    [TYPE_NAME(LAMBDA)]     = 1 << 5,
    [TYPE_NAME(APPLY)]      = 1 << 6,
    [TYPE_NAME(DEFINE)]     = 1 << 7,
    [TYPE_NAME(REGEX)]      = 1 << 8,
    [TYPE_NAME(RANGE)]      = 1 << 9,
    // TAG
    [TYPE_NAME(TAG)]        = 1 << 0,
    [TYPE_NAME(VALUE)]      = 1 << 1,
    [TYPE_NAME(ALIAS)]      = 1 << 2,
    [TYPE_NAME(VAR)]        = 1 << 3,
    [TYPE_NAME(ARG)]        = 1 << 4,
    [TYPE_NAME(NAME)]       = 1 << 5,
    // BASE
    [TYPE_NAME(BASE)]       = 1 << 0,
    [TYPE_NAME(_)]          = 0
};

uint32_t type_name_get_mask(type_name name) {
    if (name >= TYPE_NAME(_))
        return 0;
    return type_name_mask_array[name];
}

uint32_t type_name_create_mask(...) {
    uint32_t mask = 0, next = 0;
    va_list args;
    va_start(args);
    while ((next = type_name_get_mask(va_arg(args, int32_t))))
        mask |= next;
    va_end(args);
    return mask;
}

static const type_name class_first_name_array[] = {
    [TYPE_CLASS(SCALAR)]    = TYPE_NAME(VOID),
    [TYPE_CLASS(FIXED)]     = TYPE_NAME(ARRAY),
    [TYPE_CLASS(VECTOR)]    = TYPE_NAME(VECTOR),
    [TYPE_CLASS(LIST)]      = TYPE_NAME(OP),
    [TYPE_CLASS(TABLE)]     = TYPE_NAME(ENUM),
    [TYPE_CLASS(TAG)]       = TYPE_NAME(TAG),
    [TYPE_CLASS(BASE)]      = TYPE_NAME(BASE),
    [TYPE_CLASS(_)]         = TYPE_NAME(_)
};

type_name type_class_first_name(type_class class) {
    if (class >= TYPE_CLASS(_))
        return TYPE_NAME(_);
    return class_first_name_array[class];
}
