
#include "./op_name.h"

static const char *op_name_strs[] = {
    "_",
    "CMD_IS_MAIN",
    "CMD_EXPORT",
    "CMD_IMPORT",
    "CMD_LOG",
    "CMD_DEBUG",
    "CMD_TYPE",
    "CMD_RETURN_TYPE",
    "CMD_COPY",
    "CMD_DISASSEMBLE",
    "CMD_AST",
    "CMD_IR",
    "CMD_CHAR",
    "CMD_BREAK",
    "CMD_CONTINUE",
    "CMD_BITMASK",
    "CMD_RETURN",
    "CMD_YEILD",
    "CMD_VALUE",
    "CMD_ERROR",
    "CMD_REGEX",
    "INDEX",
    "INVALID"
};

const char *type_op_name_str(type_op_name op_name) {
    if (op_name >= TYPE_OP_NAME(INVALID))
        return "INVALID OP";
    return op_name_strs[op_name];
}

extern inline bool type_op_name_is_command(type_op_name op_name);
