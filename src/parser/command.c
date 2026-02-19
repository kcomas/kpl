
#include "./command.h"

#define TABLE_MATCH(CHAR, ...) \
    [CHAR - 'a'] (parser_command_match[]) { __VA_ARGS__, { .op_name = TYPE_OP_NAME(_) } }

static constexpr uint8_t lowercase_letters_values = 'z' - 'a';

static const parser_command_match *table_match[lowercase_letters_values + 1] = {
    TABLE_MATCH('a', { .c_str = "ast", .op_name = TYPE_OP_NAME(CMD_AST) }),
    TABLE_MATCH('b',
        { .c_str = "break", .op_name = TYPE_OP_NAME(CMD_BREAK) },
        { .c_str = "bitmask", .op_name = TYPE_OP_NAME(CMD_BITMASK) }
    ),
    TABLE_MATCH('c',
        { .c_str = "copy", .op_name = TYPE_OP_NAME(CMD_COPY) },
        { .c_str = "char", .op_name = TYPE_OP_NAME(CMD_CHAR) },
        { .c_str = "continue", .op_name = TYPE_OP_NAME(CMD_CONTINUE) }
    ),
    TABLE_MATCH('d',
        { .c_str = "debug", .op_name = TYPE_OP_NAME(CMD_DEBUG) },
        { .c_str = "disassemble", .op_name = TYPE_OP_NAME(CMD_DISASSEMBLE) }
    ),
    TABLE_MATCH('e',
        { .c_str = "export", .op_name = TYPE_OP_NAME(CMD_EXPORT) },
        { .c_str = "error", .op_name = TYPE_OP_NAME(CMD_ERROR) },
        { .c_str = "exit", .op_name = TYPE_OP_NAME(CMD_EXIT) }
    ),
    // f
    // g
    // h
    TABLE_MATCH('i',
        { .c_str = "is_main", .op_name = TYPE_OP_NAME(CMD_IS_MAIN) },
        { .c_str = "import", .op_name = TYPE_OP_NAME(CMD_IMPORT) },
        { .c_str = "ir", .op_name = TYPE_OP_NAME(CMD_IR) }
    ),
    // j
    TABLE_MATCH('l', { .c_str = "log", .op_name = TYPE_OP_NAME(CMD_LOG) }),
    // m
    // o
    // p
    // q
    TABLE_MATCH('r',
        { .c_str = "return_type", .op_name = TYPE_OP_NAME(CMD_RETURN_TYPE) },
        { .c_str = "return", .op_name = TYPE_OP_NAME(CMD_RETURN) },
        { .c_str = "regex", .op_name = TYPE_OP_NAME(CMD_REGEX) }
    ),
    // s
    TABLE_MATCH('t',
        { .c_str = "type", .op_name = TYPE_OP_NAME(CMD_TYPE) },
        { .c_str = "test", .op_name = TYPE_OP_NAME(CMD_TEST) }
    ),
    // u
    TABLE_MATCH('v', { .c_str = "value", .op_name = TYPE_OP_NAME(CMD_VALUE) })
    // v
    // w
    // x
    // y
    // z
};

type *parser_command(const token_slice *slice) {
    if (slice->class != TOKEN_CLASS(COMMAND) || slice->str_end - slice->str_start < 2)
        return nullptr;
    uint8_t match_value = slice->str->data[slice->str_start + sizeof(char)] - 'a';
    if (match_value > lowercase_letters_values)
        return nullptr;
    const parser_command_match *match_array = table_match[match_value];
    if (!match_array)
        return nullptr;
    while (match_array->op_name != TYPE_OP_NAME(_)) {
        if (token_slice_match_c_str(slice, match_array->c_str, sizeof(char)))
            return namespace_command(match_array->op_name, nullptr, nullptr);
        match_array++;
    }
    return nullptr;
}
