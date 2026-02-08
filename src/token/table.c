
#include "./table.h"

#define TABLE_CHARS(...) { __VA_ARGS__, '\0' }

#define TABLE_MATCH(CHAR, ...) [CHAR] = (token_table_match[]) { __VA_ARGS__, { .class = TOKEN_CLASS(_) } }

static def_status match_fixed(const token_table_match *match, token_slice *slice) {
    slice->str_end += strlen(match->str);
    return DEF_STATUS(OK);
}

static def_status match_repeat(const token_table_match *match, token_slice *slice) {
    while (slice->str->data[slice->str_end] == match->str[0])
        slice->str_end++;
    return DEF_STATUS(OK);
}

static def_status match_alphanum(token_slice *slice, int32_t offset) {
    slice->str_end += offset;
    char charater = slice->str->data[slice->str_end];
    while (isalnum(charater) || charater == '_')
        charater = slice->str->data[++slice->str_end];
    return DEF_STATUS(OK);
}

static def_status match_type(const token_table_match *match, token_slice *slice) {
    (void) match;
    return match_alphanum(slice, 0);
}

static def_status match_var(const token_table_match *match, token_slice *slice) {
    (void) match;
    return match_alphanum(slice, 0);
}

static def_status match_base(const token_table_match *match, token_slice *slice) {
    (void) match;
    return match_alphanum(slice, 1);
}

static def_status match_number(const token_table_match *match, token_slice *slice) {
    (void) match;
    char charater = slice->str->data[slice->str_end];
    while (isdigit(charater) || charater == '_')
        charater = slice->str->data[++slice->str_end];
    if (charater != '.')
        return DEF_STATUS(OK);
    slice->class = TOKEN_CLASS(FLOAT);
    while (isdigit(charater) || charater == '_')
        charater = slice->str->data[++slice->str_end];
    return charater != '.' ? DEF_STATUS(OK) : DEF_STATUS(ERROR);
}

static def_status match_string(const token_table_match *match, token_slice *slice) {
    (void) match;
    slice->str_end++;
    while (slice->str_end < slice->str->len && slice->str->data[slice->str_end] != '"')
        slice->str_end++;
    slice->str_end++;
    return slice->str_end < slice->str->len ? DEF_STATUS(OK) : DEF_STATUS(ERROR);
}

static def_status match_symbol(const token_table_match *match, token_slice *slice) {
    (void) match;
    def_status status = match_alphanum(slice, 1);
    if (status != DEF_STATUS(OK))
        return status;
    if (slice->str->data[slice->str_start + 1] == '_')
        slice->class = TOKEN_CLASS(UNUSED_SYMBOL);
    return DEF_STATUS(OK);
}

static def_status match_command(const token_table_match *match, token_slice *slice) {
    (void) match;
    return match_alphanum(slice, 1);
}

static def_status match_comment_range(const token_table_match *match, token_slice *slice) {
    (void) match;
    char charater_a, charater_b;
    while (slice->str_end + 2 < slice->str->len) {
        charater_a = slice->str->data[slice->str_end];
        charater_b = slice->str->data[slice->str_end + 1];
        slice->str_end += 2;
        if (charater_a == '\0' || charater_b == '\0')
            return DEF_STATUS(ERROR);
        if (charater_a == '*' && charater_b == '/')
            return DEF_STATUS(OK);
    }
    return DEF_STATUS(ERROR);
}

static def_status match_comment_line(const token_table_match *match, token_slice *slice) {
    (void) match;
    while (slice->str_end < slice->str->len) {
        if (slice->str->data[slice->str_end] == '\n')
            return DEF_STATUS(OK);
        slice->str_end++;
    }
    return DEF_STATUS(ERROR);
}

static const token_table_match *table_match[INT8_MAX] = {
    TABLE_MATCH('\0', { .class = TOKEN_CLASS(END), .str = TABLE_CHARS('\0'), .fn = match_fixed }),
    TABLE_MATCH(' ', { .class = TOKEN_CLASS(SPACES), .str = TABLE_CHARS(' '), .fn = match_repeat }),
    TABLE_MATCH('\t', { .class = TOKEN_CLASS(TABS), .str = TABLE_CHARS('\t'), .fn = match_repeat }),
    TABLE_MATCH('\n', { .class = TOKEN_CLASS(NEW_LINE), .str = TABLE_CHARS('\n'), .fn = match_fixed }),
    TABLE_MATCH(';', { .class = TOKEN_CLASS(SEMICOLON), .str = TABLE_CHARS(';'), .fn = match_fixed }),
    TABLE_MATCH('A', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('A'), .fn = match_type }),
    TABLE_MATCH('B', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('B'), .fn = match_type }),
    TABLE_MATCH('C', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('C'), .fn = match_type }),
    TABLE_MATCH('D', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('D'), .fn = match_type }),
    TABLE_MATCH('E', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('E'), .fn = match_type }),
    TABLE_MATCH('F',
        { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('F'), .fn = match_type },
        { .class = TOKEN_CLASS(FALSE_CONST), .str = TABLE_CHARS('F', 'a', 'l', 's', 'e'), .fn = match_fixed }
    ),
    TABLE_MATCH('G', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('G'), .fn = match_type }),
    TABLE_MATCH('H', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('H'), .fn = match_type }),
    TABLE_MATCH('I', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('I'), .fn = match_type }),
    TABLE_MATCH('J', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('J'), .fn = match_type }),
    TABLE_MATCH('K', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('K'), .fn = match_type }),
    TABLE_MATCH('L', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('L'), .fn = match_type }),
    TABLE_MATCH('M', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('M'), .fn = match_type }),
    TABLE_MATCH('N', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('N'), .fn = match_type }),
    TABLE_MATCH('O', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('O'), .fn = match_type }),
    TABLE_MATCH('P', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('P'), .fn = match_type }),
    TABLE_MATCH('Q', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('Q'), .fn = match_type }),
    TABLE_MATCH('R', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('R'), .fn = match_type }),
    TABLE_MATCH('S', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('S'), .fn = match_type }),
    TABLE_MATCH('T',
        { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('T'), .fn = match_type },
        { .class = TOKEN_CLASS(TRUE_CONST), .str = TABLE_CHARS('T', 'r', 'u', 'e'), .fn = match_fixed }
    ),
    TABLE_MATCH('U', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('U'), .fn = match_type }),
    TABLE_MATCH('V', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('V'), .fn = match_type }),
    TABLE_MATCH('W', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('W'), .fn = match_type }),
    TABLE_MATCH('X', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('X'), .fn = match_type }),
    TABLE_MATCH('Y', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('Y'), .fn = match_type }),
    TABLE_MATCH('Z', { .class = TOKEN_CLASS(TYPE), .str = TABLE_CHARS('Z'), .fn = match_type }),
    TABLE_MATCH('a', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('a'), .fn = match_var }),
    TABLE_MATCH('b', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('b'), .fn = match_var }),
    TABLE_MATCH('c', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('c'), .fn = match_var }),
    TABLE_MATCH('d', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('d'), .fn = match_var }),
    TABLE_MATCH('e', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('e'), .fn = match_var }),
    TABLE_MATCH('f',
        { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('f'), .fn = match_var },
        { .class = TOKEN_CLASS(FALSE_VALUE), .str = TABLE_CHARS('f', 'a', 'l', 's', 'e'), .fn = match_fixed }
    ),
    TABLE_MATCH('g', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('g'), .fn = match_var }),
    TABLE_MATCH('h', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('h'), .fn = match_var }),
    TABLE_MATCH('i', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('i'), .fn = match_var }),
    TABLE_MATCH('j', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('j'), .fn = match_var }),
    TABLE_MATCH('k', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('k'), .fn = match_var }),
    TABLE_MATCH('l', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('l'), .fn = match_var }),
    TABLE_MATCH('m', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('m'), .fn = match_var }),
    TABLE_MATCH('n', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('n'), .fn = match_var }),
    TABLE_MATCH('o', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('o'), .fn = match_var }),
    TABLE_MATCH('p', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('p'), .fn = match_var }),
    TABLE_MATCH('q', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('q'), .fn = match_var }),
    TABLE_MATCH('r', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('r'), .fn = match_var }),
    TABLE_MATCH('s', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('s'), .fn = match_var }),
    TABLE_MATCH('t',
        { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('t'), .fn = match_var },
        { .class = TOKEN_CLASS(TRUE_VALUE), .str = TABLE_CHARS('t', 'r', 'u', 'e'), .fn = match_fixed }
    ),
    TABLE_MATCH('u', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('u'), .fn = match_var }),
    TABLE_MATCH('v', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('v'), .fn = match_var }),
    TABLE_MATCH('w', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('w'), .fn = match_var }),
    TABLE_MATCH('x', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('x'), .fn = match_var }),
    TABLE_MATCH('y', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('y'), .fn = match_var }),
    TABLE_MATCH('z', { .class = TOKEN_CLASS(VAR), .str = TABLE_CHARS('z'), .fn = match_var }),
    TABLE_MATCH('_', { .class = TOKEN_CLASS(UNUSED_VAR), .str = TABLE_CHARS('_'), .fn = match_var }),
    TABLE_MATCH('.',
        { .class = TOKEN_CLASS(BASE), .str = TABLE_CHARS('.'), .fn = match_base },
        { .class = TOKEN_CLASS(PERIOD_DOUBLE), .str = TABLE_CHARS('.', '.'), .fn = match_fixed }
    ),
    TABLE_MATCH('0', { .class = TOKEN_CLASS(INTEGER), .str = TABLE_CHARS('0'), .fn = match_number }),
    TABLE_MATCH('1', { .class = TOKEN_CLASS(INTEGER), .str = TABLE_CHARS('1'), .fn = match_number }),
    TABLE_MATCH('2', { .class = TOKEN_CLASS(INTEGER), .str = TABLE_CHARS('2'), .fn = match_number }),
    TABLE_MATCH('3', { .class = TOKEN_CLASS(INTEGER), .str = TABLE_CHARS('3'), .fn = match_number }),
    TABLE_MATCH('4', { .class = TOKEN_CLASS(INTEGER), .str = TABLE_CHARS('4'), .fn = match_number }),
    TABLE_MATCH('5', { .class = TOKEN_CLASS(INTEGER), .str = TABLE_CHARS('5'), .fn = match_number }),
    TABLE_MATCH('6', { .class = TOKEN_CLASS(INTEGER), .str = TABLE_CHARS('6'), .fn = match_number }),
    TABLE_MATCH('7', { .class = TOKEN_CLASS(INTEGER), .str = TABLE_CHARS('7'), .fn = match_number }),
    TABLE_MATCH('8', { .class = TOKEN_CLASS(INTEGER), .str = TABLE_CHARS('8'), .fn = match_number }),
    TABLE_MATCH('9', { .class = TOKEN_CLASS(INTEGER), .str = TABLE_CHARS('9'), .fn = match_number }),
    TABLE_MATCH('"', { .class = TOKEN_CLASS(STRING), .str = TABLE_CHARS('"'), .fn = match_string }),
    TABLE_MATCH('`', { .class = TOKEN_CLASS(SYMBOL), .str = TABLE_CHARS('`'), .fn = match_symbol }),
    TABLE_MATCH('\\', { .class = TOKEN_CLASS(COMMAND), .str = TABLE_CHARS('\\'), .fn = match_command }),
    TABLE_MATCH('(', { .class = TOKEN_CLASS(APPLY_START), .str = TABLE_CHARS('('), .fn = match_fixed }),
    TABLE_MATCH(')', { .class = TOKEN_CLASS(APPLY_END), .str = TABLE_CHARS(')'), .fn = match_fixed }),
    TABLE_MATCH('[', { .class = TOKEN_CLASS(DEFINE_START), .str = TABLE_CHARS('['), .fn = match_fixed }),
    TABLE_MATCH(']', { .class = TOKEN_CLASS(DEFINE_END), .str = TABLE_CHARS(']'), .fn = match_fixed }),
    TABLE_MATCH('{', { .class = TOKEN_CLASS(LAMBDA_START), .str = TABLE_CHARS('{'), .fn = match_fixed }),
    TABLE_MATCH('}', { .class = TOKEN_CLASS(LAMBDA_END), .str = TABLE_CHARS('}'), .fn = match_fixed }),
    TABLE_MATCH('~', { .class = TOKEN_CLASS(TILDE), .str = TABLE_CHARS('~'), .fn = match_fixed }),
    TABLE_MATCH('!',
        { .class = TOKEN_CLASS(EXCLAMATION), .str = TABLE_CHARS('!'), .fn = match_fixed },
        { .class = TOKEN_CLASS(EXCLAMATION_COLON), .str = TABLE_CHARS('!', ':'), .fn = match_fixed },
        { .class = TOKEN_CLASS(EXCLAMATION_EQUAL), .str = TABLE_CHARS('!', '='), .fn = match_fixed },
        { .class = TOKEN_CLASS(EXCLAMATION_DOUBLE), .str = TABLE_CHARS('!', '!'), .fn = match_fixed },
        { .class = TOKEN_CLASS(EXCLAMATION_DOUBLE_COLON), .str = TABLE_CHARS('!', '!', ':'), .fn = match_fixed }
    ),
    TABLE_MATCH('#', { .class = TOKEN_CLASS(POUND), .str = TABLE_CHARS('#'), .fn = match_fixed }),
    TABLE_MATCH('@', { .class = TOKEN_CLASS(AT), .str = TABLE_CHARS('@'), .fn = match_fixed }),
    TABLE_MATCH('$',
        { .class = TOKEN_CLASS(DOLLAR), .str = TABLE_CHARS('$'), .fn = match_fixed },
        { .class = TOKEN_CLASS(DOLLAR_COLON), .str = TABLE_CHARS('$', ':'), .fn = match_fixed }
    ),
    TABLE_MATCH('%',
        { .class = TOKEN_CLASS(PERCENT), .str = TABLE_CHARS('%'), .fn = match_fixed },
        { .class = TOKEN_CLASS(PERCENT_COLON), .str = TABLE_CHARS('%', ':'), .fn = match_fixed }
    ),
    TABLE_MATCH('^',
        { .class = TOKEN_CLASS(CARET), .str = TABLE_CHARS('^'), .fn = match_fixed },
        { .class = TOKEN_CLASS(CARET_DOUBLE), .str = TABLE_CHARS('^', '^'), .fn = match_fixed },
        { .class = TOKEN_CLASS(CARET_DOUBLE_COLON), .str = TABLE_CHARS('^', '^', ':'), .fn = match_fixed }
    ),
    TABLE_MATCH('&',
        { .class = TOKEN_CLASS(AMPERSAND), .str = TABLE_CHARS('&'), .fn = match_fixed },
        { .class = TOKEN_CLASS(AMPERSAND_DOUBLE), .str = TABLE_CHARS('&', '&'), .fn = match_fixed },
        { .class = TOKEN_CLASS(AMPERSAND_DOUBLE_COLON), .str = TABLE_CHARS('&', '&', ':'), .fn = match_fixed }
    ),
    TABLE_MATCH('*',
        { .class = TOKEN_CLASS(STAR), .str = TABLE_CHARS('*'), .fn = match_fixed },
        { .class = TOKEN_CLASS(STAR_COLON), .str = TABLE_CHARS('*', ':'), .fn = match_fixed },
        { .class = TOKEN_CLASS(STAR_DOUBLE), .str = TABLE_CHARS('*', '*'), .fn = match_fixed },
        { .class = TOKEN_CLASS(STAR_DOUBLE_COLON), .str = TABLE_CHARS('*', '*', ':'), .fn = match_fixed }
    ),
    TABLE_MATCH('-',
        { .class = TOKEN_CLASS(DASH), .str = TABLE_CHARS('-'), .fn = match_fixed },
        { .class = TOKEN_CLASS(DASH_COLON), .str = TABLE_CHARS('-', ':'), .fn = match_fixed }
    ),
    TABLE_MATCH('+',
        { .class = TOKEN_CLASS(PLUS), .str = TABLE_CHARS('+'), .fn = match_fixed },
        { .class = TOKEN_CLASS(PLUS_COLON), .str = TABLE_CHARS('+', ':'), .fn = match_fixed }
    ),
    TABLE_MATCH('=',
        { .class = TOKEN_CLASS(EQUAL), .str = TABLE_CHARS('='), .fn = match_fixed },
        { .class = TOKEN_CLASS(EQUAL_DOUBLE), .str = TABLE_CHARS('=', '='), .fn = match_fixed }
    ),
    TABLE_MATCH('|',
        { .class = TOKEN_CLASS(PIPE), .str = TABLE_CHARS('|'), .fn = match_fixed },
        { .class = TOKEN_CLASS(PIPE_DOUBLE), .str = TABLE_CHARS('|', '|'), .fn = match_fixed },
        { .class = TOKEN_CLASS(PIPE_DOUBLE_COLON), .str = TABLE_CHARS('|', '|', ':'), .fn = match_fixed }
    ),
    TABLE_MATCH(':',
        { .class = TOKEN_CLASS(COLON), .str = TABLE_CHARS(':'), .fn = match_fixed },
        { .class = TOKEN_CLASS(COLON_DOUBLE), .str = TABLE_CHARS(':', ':'), .fn = match_fixed }
    ),
    TABLE_MATCH('\'', { .class = TOKEN_CLASS(QUOTE), .str = TABLE_CHARS('\''), .fn = match_fixed }),
    TABLE_MATCH('<',
        { .class = TOKEN_CLASS(ARROW_LEFT), .str = TABLE_CHARS('<'), .fn = match_fixed },
        { .class = TOKEN_CLASS(ARROW_LEFT_COLON_ARROW_RIGHT),
            .str = TABLE_CHARS('<', ':', '>'), .fn = match_fixed },
        { .class = TOKEN_CLASS(ARROW_LEFT_EQUAL), .str = TABLE_CHARS('<', '='), .fn = match_fixed },
        { .class = TOKEN_CLASS(ARROW_LEFT_EQUAL_ARROW_RIGHT),
            .str = TABLE_CHARS('<', '=', '>'), .fn = match_fixed },
        { .class = TOKEN_CLASS(ARROW_LEFT_DOUBLE), .str = TABLE_CHARS('<', '<'), .fn = match_fixed },
        { .class = TOKEN_CLASS(ARROW_LEFT_DOUBLE_COLON), .str = TABLE_CHARS('<', '<', ':'), .fn = match_fixed }
    ),
    TABLE_MATCH(',',
        { .class = TOKEN_CLASS(COMMA), .str = TABLE_CHARS(','), .fn = match_fixed },
        { .class = TOKEN_CLASS(COMMA_COLON), .str = TABLE_CHARS(',', ':'), .fn = match_fixed }
    ),
    TABLE_MATCH('>',
        { .class = TOKEN_CLASS(ARROW_RIGHT), .str = TABLE_CHARS('>'), .fn = match_fixed },
        { .class = TOKEN_CLASS(ARROW_RIGHT_EQUAL), .str = TABLE_CHARS('>', '='), .fn = match_fixed },
        { .class = TOKEN_CLASS(ARROW_RIGHT_DOUBLE), .str = TABLE_CHARS('>', '>'), .fn = match_fixed },
        { .class = TOKEN_CLASS(ARROW_RIGHT_DOUBLE_COLON), .str = TABLE_CHARS('>', '>', ':'), .fn = match_fixed }
    ),
    TABLE_MATCH('?', { .class = TOKEN_CLASS(QUESTION), .str = TABLE_CHARS('?'), .fn = match_fixed }),
    TABLE_MATCH('/',
        { .class = TOKEN_CLASS(FOWARD_SLASH), .str = TABLE_CHARS('/'), .fn = match_fixed },
        { .class = TOKEN_CLASS(FOWARD_SLASH_COLON), .str = TABLE_CHARS('/', ':'), .fn = match_fixed },
        { .class = TOKEN_CLASS(COMMENT_RANGE), .str = TABLE_CHARS('/', '*'), .fn = match_comment_range },
        { .class = TOKEN_CLASS(COMMENT_LINE), .str = TABLE_CHARS('/', '/'), .fn = match_comment_line }
    ),
};

def_status token_table_match_next(token_slice *slice) {
    const token_table_match *match_array = table_match[(uint8_t) slice->str->data[slice->str_end]];
    if (!match_array)
        return DEF_STATUS(ERROR);
    const token_table_match *match = nullptr;
    while (match_array->class != TOKEN_CLASS(_)) {
        uint32_t match_len = 0;
        while (slice->str->data[slice->str_end + match_len] == match_array->str[match_len] &&
                match_array->str[match_len])
            match_len++;
        if (!match_array->str[match_len])
            match = match_array;
        match_array++;
    }
    if (!match)
        return DEF_STATUS(ERROR);
    slice->class = match->class;
    return match->fn(match, slice);
}
