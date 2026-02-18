
#include "./type.h"

#define TABLE_MATCH(CHAR, ...) \
    [CHAR - 'A'] = (parser_type_match[]) {  \
        __VA_ARGS__, \
        { .name = TYPE_NAME(_), .qualifier = TYPE_QUALIFIER_FLAG(_) } }

static constexpr uint8_t uppercase_letters_values = 'Z' - 'A';

static const parser_type_match *table_match[uppercase_letters_values + 1] = {
    TABLE_MATCH('A',
        { .c_str = "Array", .name = TYPE_NAME(ARRAY), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "Apply", .name = TYPE_NAME(APPLY), .qualifier = TYPE_QUALIFIER_FLAG(_) }
    ),
    TABLE_MATCH('B',
        { .c_str = "Bool", .name = TYPE_NAME(BOOL), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "Buffer", .name = TYPE_NAME(BUFFER), .qualifier = TYPE_QUALIFIER_FLAG(_) }
    ),
    TABLE_MATCH('C',
        { .c_str = "Char", .name = TYPE_NAME(CHAR), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "Closure", .name = TYPE_NAME(CLOSURE), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "Const", .name = TYPE_NAME(_), .qualifier = TYPE_QUALIFIER(CONST) }
    ),
    // D
    TABLE_MATCH('E', { .c_str = "Enum", .name = TYPE_NAME(ENUM), .qualifier = TYPE_QUALIFIER_FLAG(_) }),
    TABLE_MATCH('F',
        { .c_str = "F32", .name = TYPE_NAME(F32), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "F64", .name = TYPE_NAME(F64), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "Fd", .name = TYPE_NAME(FD), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "Fn", .name = TYPE_NAME(FN), .qualifier = TYPE_QUALIFIER_FLAG(_) }
    ),
    // G
    // H
    TABLE_MATCH('I',
        { .c_str = "I8", .name = TYPE_NAME(I8), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "I16", .name = TYPE_NAME(I16), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "I32", .name = TYPE_NAME(I32), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "I64", .name = TYPE_NAME(I64), .qualifier = TYPE_QUALIFIER_FLAG(_) }
    ),
    // J
    // K
    TABLE_MATCH('L', { .c_str = "Lambda", .name = TYPE_NAME(LAMBDA), .qualifier = TYPE_QUALIFIER_FLAG(_) }),
    TABLE_MATCH('M',
        { .c_str = "Map", .name = TYPE_NAME(MAP), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "Mutex", .name = TYPE_NAME(_), .qualifier = TYPE_QUALIFIER(MUTEX) }
    ),
    // N
    TABLE_MATCH('O',
        { .c_str = "Option", .name = TYPE_NAME(OPTION), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "Overload", .name = TYPE_NAME(OVERLOAD), .qualifier = TYPE_QUALIFIER_FLAG(_) }
    ),
    // P
    // Q
    TABLE_MATCH('R',
        { .c_str = "Result", .name = TYPE_NAME(RESULT), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "Regex", .name = TYPE_NAME(REGEX), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "Range", .name = TYPE_NAME(RANGE), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "Ref", .name = TYPE_NAME(_), .qualifier = TYPE_QUALIFIER(REF) }
    ),
    TABLE_MATCH('S',
        { .c_str = "String", .name = TYPE_NAME(STRING), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "Socket", .name = TYPE_NAME(SOCKET), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "Set", .name = TYPE_NAME(SET), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "Struct", .name = TYPE_NAME(STRUCT), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "Shared", .name = TYPE_NAME(_), .qualifier = TYPE_QUALIFIER(SHARED) }
    ),
    TABLE_MATCH('T', { .c_str = "Tuple", .name = TYPE_NAME(TUPLE), .qualifier = TYPE_QUALIFIER_FLAG(_) }),
    TABLE_MATCH('U',
        { .c_str = "U8", .name = TYPE_NAME(U8), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "U16", .name = TYPE_NAME(U16), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "U32", .name = TYPE_NAME(U32), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "U64", .name = TYPE_NAME(U64), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "Union", .name = TYPE_NAME(UNION), .qualifier = TYPE_QUALIFIER_FLAG(_) }
    ),
    TABLE_MATCH('V',
        { .c_str = "Void", .name = TYPE_NAME(VOID), .qualifier = TYPE_QUALIFIER_FLAG(_) },
        { .c_str = "Vector", .name = TYPE_NAME(VECTOR), .qualifier = TYPE_QUALIFIER_FLAG(_) }
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

type *parser_type(const token_slice *slice) {
    if (slice->class != TOKEN_CLASS(TYPE))
        return nullptr;
    uint8_t match_value = slice->str->data[slice->str_start] - 'A';
    if (match_value > uppercase_letters_values)
        return nullptr;
    const parser_type_match *match_array = table_match[match_value];
    if (!match_array)
        return parser_type_alias(slice);
    while (match_array->name != TYPE_NAME(_) || match_array->qualifier != TYPE_QUALIFIER_FLAG(_)) {
        if (token_slice_match_c_str(slice, match_array->c_str, 0))
            return type_init(match_array->name, match_array->qualifier, type_class_union_empty());
        match_array++;
    }
    return parser_type_alias(slice);
}
