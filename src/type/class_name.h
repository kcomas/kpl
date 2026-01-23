
#pragma once

#define TYPE_CLASS(NAME) TYPE_CLASS_##NAME

typedef enum [[gnu::packed]] {
    TYPE_CLASS(SCALAR),
    TYPE_CLASS(FIXED),
    TYPE_CLASS(VECTOR),
    TYPE_CLASS(LIST),
    TYPE_CLASS(TABLE),
    TYPE_CLASS(TAG),
    TYPE_CLASS(BASE),
    TYPE_CLASS(_)
} type_class;

#define TYPE_NAME(NAME) TYPE_NAME_##NAME

typedef enum [[gnu::packed]] {
    // SCALAR
    TYPE_NAME(VOID),
    TYPE_NAME(BOOL),
    TYPE_NAME(U8),
    TYPE_NAME(U16),
    TYPE_NAME(U32),
    TYPE_NAME(U64),
    TYPE_NAME(I8),
    TYPE_NAME(I16),
    TYPE_NAME(I32),
    TYPE_NAME(I64),
    TYPE_NAME(F32),
    TYPE_NAME(F64),
    TYPE_NAME(CHAR),
    TYPE_NAME(STRING),
    TYPE_NAME(REGEX),
    TYPE_NAME(ERROR),
    TYPE_NAME(FD),
    TYPE_NAME(SOCKET),
    // SIZE
    TYPE_NAME(ARRAY),
    TYPE_NAME(TMP),
    // VECTOR
    TYPE_NAME(VECTOR),
    TYPE_NAME(SET),
    TYPE_NAME(RESULT),
    TYPE_NAME(OPTION),
    TYPE_NAME(CLOSURE),
    TYPE_NAME(THREAD),
    // LIST
    TYPE_NAME(OP),
    TYPE_NAME(MAP),
    TYPE_NAME(OVERLOAD),
    // TABLE
    TYPE_NAME(ENUM),
    TYPE_NAME(STRUCT),
    TYPE_NAME(UNION),
    TYPE_NAME(FN),
    TYPE_NAME(COROUTINE),
    TYPE_NAME(LAMBDA),
    // TAG
    TYPE_NAME(TAG),
    TYPE_NAME(VALUE),
    TYPE_NAME(ALIAS),
    TYPE_NAME(VAR),
    TYPE_NAME(ARG),
    // BASE
    TYPE_NAME(BASE),
    TYPE_NAME(_)
} type_name;

const char *type_name_str(type_name name);

type_class type_name_get_class(type_name name);
