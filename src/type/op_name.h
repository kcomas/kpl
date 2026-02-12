
#pragma once

#define TYPE_OP_NAME(NAME) TYPE_OP_NAME_##NAME

typedef enum [[gnu::packed]] {
    TYPE_OP_NAME(_),
    // placeholders before type checking
    // commands
    TYPE_OP_NAME(CMD_IS_MAIN),
    TYPE_OP_NAME(CMD_EXPORT),
    TYPE_OP_NAME(CMD_IMPORT),
    TYPE_OP_NAME(CMD_LOG),
    TYPE_OP_NAME(CMD_DEBUG),
    TYPE_OP_NAME(CMD_TYPE),
    TYPE_OP_NAME(CMD_RETURN_TYPE),
    TYPE_OP_NAME(CMD_COPY),
    TYPE_OP_NAME(CMD_DISASSEMBLE),
    TYPE_OP_NAME(CMD_AST),
    TYPE_OP_NAME(CMD_IR),
    TYPE_OP_NAME(CMD_CHAR),
    TYPE_OP_NAME(CMD_BREAK),
    TYPE_OP_NAME(CMD_CONTINUE),
    TYPE_OP_NAME(CMD_BITMASK),
    TYPE_OP_NAME(CMD_RETURN),
    TYPE_OP_NAME(CMD_YEILD),
    TYPE_OP_NAME(CMD_VALUE),
    TYPE_OP_NAME(CMD_ERROR),
    TYPE_OP_NAME(CMD_REGEX),
    // general
    TYPE_OP_NAME(INDEX),
    TYPE_OP_NAME(INVALID)
} type_op_name;

const char *type_op_name_str(type_op_name op_name);

inline bool type_op_name_is_command(type_op_name op_name) {
    return op_name >= TYPE_OP_NAME(CMD_IS_MAIN) && op_name <= TYPE_OP_NAME(CMD_REGEX);
}
