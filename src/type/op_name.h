
#pragma once

#define TYPE_OP_NAME(NAME) TYPE_OP_NAME_##NAME

typedef enum [[gnu::packed]] {
    TYPE_OP_NAME(_),
    TYPE_OP_NAME(INDEX),
    TYPE_OP_NAME(INVALID)
} type_op_name;

const char *type_op_name_str(type_op_name op_name);
