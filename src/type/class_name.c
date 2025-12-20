
#include "./class_name.h"

static const char *name_str_array[] = {
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
    "REGEX",
    "ERROR",
    "FD",
    "SOCKET",
    "ARRAY",
    "TMP",
    "VECTOR",
    "SET",
    "RESULT",
    "OPTION",
    "CLOSURE",
    "THREAD",
    "OP",
    "MAP",
    "OVERLOAD",
    "ENUM",
    "STRUCT",
    "UNION",
    "BASE",
    "FN",
    "COROUTINE",
    "LAMBDA",
    "TAG",
    "ALIAS",
    "VAR",
    "ARG",
    "_"
};

const char *type_name_str(type_name name) {
    if (name >= TYPE_NAME(_))
        return "INVALID TYPE NAME";
    return name_str_array[name];
}

#define CLASS_NAME(CLASS, NAME) [TYPE_NAME(NAME)] = TYPE_CLASS(CLASS)

static type_class type_name_class_array[] = {
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
    CLASS_NAME(SCALAR, REGEX),
    CLASS_NAME(SCALAR, ERROR),
    CLASS_NAME(SCALAR, FD),
    CLASS_NAME(SCALAR, SOCKET),
    CLASS_NAME(FIXED, ARRAY),
    CLASS_NAME(FIXED, TMP),
    CLASS_NAME(VECTOR, VECTOR),
    CLASS_NAME(VECTOR, SET),
    CLASS_NAME(VECTOR, RESULT),
    CLASS_NAME(VECTOR, OPTION),
    CLASS_NAME(VECTOR, CLOSURE),
    CLASS_NAME(VECTOR, THREAD),
    CLASS_NAME(LIST, OP),
    CLASS_NAME(LIST, MAP),
    CLASS_NAME(LIST, OVERLOAD),
    CLASS_NAME(TABLE, ENUM),
    CLASS_NAME(TABLE, STRUCT),
    CLASS_NAME(TABLE, UNION),
    CLASS_NAME(TABLE, BASE),
    CLASS_NAME(TABLE, FN),
    CLASS_NAME(TABLE, COROUTINE),
    CLASS_NAME(TABLE, LAMBDA),
    CLASS_NAME(TAG, TAG),
    CLASS_NAME(TAG, ALIAS),
    CLASS_NAME(TAG, VAR),
    CLASS_NAME(TAG, ARG)
};

type_class type_name_get_class(type_name name) {
    if (name >= TYPE_NAME(_))
        return TYPE_CLASS(_);
    return type_name_class_array[name];
}
