
#include "./type.h"

#define TABLE_MATCH(CHAR, ...) \
    [CHAR - 'A'] = (parser_type_match[]) { __VA_ARGS__, { .name = TYPE_NAME(_) } }

static constexpr uint8_t uppercase_letters_count = 'Z' - 'A';

static const parser_type_match *table_match[uppercase_letters_count + 1] = {
    TABLE_MATCH('A',
        { .c_str = "Array", .name = TYPE_NAME(ARRAY) },
        { .c_str = "Apply", .name = TYPE_NAME(APPLY) }
    ),
    TABLE_MATCH('B',
        { .c_str = "Bool", .name = TYPE_NAME(BOOL) },
        { .c_str = "Buffer", .name = TYPE_NAME(BUFFER) }
    ),
    TABLE_MATCH('C',
        { .c_str = "Char", .name = TYPE_NAME(CHAR) },
        { .c_str = "Closure", .name = TYPE_NAME(CLOSURE) }
    ),
    // D
    TABLE_MATCH('E', { .c_str = "Enum", .name = TYPE_NAME(ENUM) }),
    TABLE_MATCH('F',
        { .c_str = "F32", .name = TYPE_NAME(F32) },
        { .c_str = "F64", .name = TYPE_NAME(F64) },
        { .c_str = "Fd", .name = TYPE_NAME(FD) },
        { .c_str = "Fn", .name = TYPE_NAME(FN) }
    ),
    // G
    // H
    TABLE_MATCH('I',
        { .c_str = "I8", .name = TYPE_NAME(I8), },
        { .c_str = "I16", .name = TYPE_NAME(I16) },
        { .c_str = "I32", .name = TYPE_NAME(I32) },
        { .c_str = "I64", .name = TYPE_NAME(I64) }
    ),
    // J
    // K
    TABLE_MATCH('L', { .c_str = "Lambda", .name = TYPE_NAME(LAMBDA) }),
    TABLE_MATCH('M', { .c_str = "Map", .name = TYPE_NAME(MAP) }),
    // N
    TABLE_MATCH('O',
        { .c_str = "Option", .name = TYPE_NAME(OPTION) },
        { .c_str = "Overload", .name = TYPE_NAME(OVERLOAD) }
    ),
    // P
    // Q
    TABLE_MATCH('R',
        { .c_str = "Result", .name = TYPE_NAME(RESULT) },
        { .c_str = "Regex", .name = TYPE_NAME(REGEX) },
        { .c_str = "Range", .name = TYPE_NAME(RANGE) }
    ),
    TABLE_MATCH('S',
        { .c_str = "String", .name = TYPE_NAME(STRING) },
        { .c_str = "Socket", .name = TYPE_NAME(SOCKET) },
        { .c_str = "Set", .name = TYPE_NAME(SET) },
        { .c_str = "Struct", .name = TYPE_NAME(STRUCT) }
    ),
    TABLE_MATCH('T', { .c_str = "Tuple", .name = TYPE_NAME(TUPLE) }),
    TABLE_MATCH('U',
        { .c_str = "U8", .name = TYPE_NAME(U8) },
        { .c_str = "U16", .name = TYPE_NAME(U16) },
        { .c_str = "U32", .name = TYPE_NAME(U32) },
        { .c_str = "U64", .name = TYPE_NAME(U64) },
        { .c_str = "Union", .name = TYPE_NAME(UNION) }
    ),
    TABLE_MATCH('V',
        { .c_str = "Void", .name = TYPE_NAME(VOID) },
        { .c_str = "Vector", .name = TYPE_NAME(VECTOR) }
    )
    // W
    // X
    // Y
    // Z
};

static type *parser_type_alias(const token_slice *slice) {
    type_tag *tag = type_tag_init(nullptr, 0, &def_unused_fn_table, def());
    if (type_tag_symbol_from_c_str_slice(tag, slice->str->data + slice->str_start,
                slice->str_end - slice->str_start) != DEF_STATUS(OK)) {
        type_tag_free(tag);
        return nullptr;
    }
    return type_init(TYPE_NAME(ALIAS), TYPE_QUALIFIER_FLAG(_), type_class_union_tag(tag));
}

static type *parser_type_match_c_str(const parser_type_match *match, const token_slice *slice) {
    const char *c_str = match->c_str;
    uint32_t str_idx = slice->str_start;
    while (c_str && str_idx < slice->str_end) {
        if (*c_str != slice->str->data[str_idx])
            break;
        c_str++;
        str_idx++;
    }
    if (!*c_str && str_idx == slice->str_end)
        return type_init(match->name, TYPE_QUALIFIER_FLAG(_), type_class_union_empty());
    return nullptr;
}

type *parser_type(const token_slice *slice) {
    if (slice->class != TOKEN_CLASS(TYPE))
        return nullptr;
    uint8_t match_value = slice->str->data[slice->str_start] - 'A';
    if (match_value > uppercase_letters_count)
        return nullptr;
    const parser_type_match *match_array = table_match[match_value];
    if (!match_array)
        return parser_type_alias(slice);
    while (match_array->name != TYPE_NAME(_)) {
        type *found_type = parser_type_match_c_str(match_array, slice);
        if (found_type)
            return found_type;
        match_array++;
    }
    return parser_type_alias(slice);
}
