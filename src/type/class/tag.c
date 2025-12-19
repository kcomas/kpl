
#include "./tag.h"

MEM_POOL(tag_pool);

type_tag *type_tag_init(type *inner_type, uint32_t print_opts, def_fn_table *fn_table, def_data data) {
    type_tag *tag = mem_alloc(&tag_pool, sizeof(type_tag));
    tag->print_opts = print_opts;
    tag->fn_table = fn_table;
    tag->data = data;
    tag->inner_type = inner_type;
    tag->stk_reg_id = -1;
    memset(&tag->symbol, 0, sizeof(uint64_t) * TYPE_SYMBOL_PARTS);
    return tag;
}

void type_tag_free(type_tag *tag) {
    if (tag->inner_type)
        type_free(tag->inner_type);
    if (tag->fn_table->free_fn && tag->data.ptr)
        tag->fn_table->free_fn(tag->data.ptr);
    mem_free(&tag_pool, tag);
}

#define NUMBER_RANGE_START 1

#define UPPER_RANGE_START 12

#define LOWER_RANGE_START 38

static uint64_t symbol_value_from_char(char c) {
    if (c == '_')
        return 0;
    if (c >= '0' && c <= '9')
        return c - '0' + NUMBER_RANGE_START;
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + UPPER_RANGE_START;
    return c - 'a' + LOWER_RANGE_START;
}

static def_status symbol_store_part(uint64_t symbol[TYPE_SYMBOL_PARTS], size_t part_idx, const char *c_str,
    size_t str_len) {
    uint64_t part_value;
    for (part_value = 0; part_value < TYPE_SYMBOL_VALUES_PER_PART &&
        str_len > TYPE_SYMBOL_VALUES_PER_PART * part_idx + part_value; part_value++) {
        char c = *c_str++;
        if (!isalnum(c) && c != '_')
            return DEF_STATUS(ERROR);
        symbol[part_idx] |= symbol_value_from_char(c) << (part_value * TYPE_SYMBOL_BITS_PER_VALUE);
    }
    symbol[part_idx] &= ~(DEF_U64_MASK(60) | DEF_U64_MASK(61) | DEF_U64_MASK(62) | DEF_U64_MASK(63));
    symbol[part_idx] |= part_value << (TYPE_SYMBOL_BITS_PER_VALUE * TYPE_SYMBOL_VALUES_PER_PART);
    return DEF_STATUS(OK);
}

def_status type_tag_symbol_from_c_str_slice(type_tag *tag, const char *c_str, size_t str_len) {
    if (!str_len || str_len > TYPE_SYMBOL_LEN)
        return DEF_STATUS(ERROR);
    for (size_t part_idx = 0; part_idx < TYPE_SYMBOL_PARTS; part_idx++) {
        tag->symbol[part_idx] = 0;
        def_status status;
        if ((status = symbol_store_part(tag->symbol, part_idx, c_str, str_len)) != DEF_STATUS(OK))
            return status;
        c_str += TYPE_SYMBOL_VALUES_PER_PART;
    }
    return DEF_STATUS(OK);
}

static char symbol_char_from_value(uint8_t value) {
    if (!value)
        return '_';
    if (value < UPPER_RANGE_START)
        return value - NUMBER_RANGE_START + '0';
    if (value < LOWER_RANGE_START)
        return value - UPPER_RANGE_START + 'A';
    return value - LOWER_RANGE_START + 'a';
}

static void symbol_load_part(uint64_t part, size_t part_idx, uint8_t buffer[TYPE_SYMBOL_C_STR_SIZE]) {
    size_t part_len = part >> (TYPE_SYMBOL_BITS_PER_VALUE * TYPE_SYMBOL_VALUES_PER_PART);
    for (size_t part_value = 0; part_value < part_len; part_value++) {
        char c = symbol_char_from_value(part & (1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5));
        buffer[part_idx * TYPE_SYMBOL_VALUES_PER_PART + part_value] = c;
        part >>= TYPE_SYMBOL_BITS_PER_VALUE;
    }
}

def_status type_tag_symbol_to_byte_array(const type_tag *tag, uint8_t buffer[TYPE_SYMBOL_C_STR_SIZE]) {
    if (!tag->symbol[0])
        return DEF_STATUS(ERROR);
    for (size_t part_idx = 0; part_idx < TYPE_SYMBOL_PARTS; part_idx++) {
        if (!tag->symbol[part_idx])
            break;
        symbol_load_part(tag->symbol[part_idx], part_idx, buffer);
    }
    return DEF_STATUS(OK);
}

size_t type_tag_hash(const type_tag *tag) {
    size_t hash = tag->fn_table->hash_fn(tag->data) + type_hash(tag->inner_type);
    for (size_t part_idx = 0; part_idx < TYPE_SYMBOL_PARTS; part_idx++)
        hash += def_hash64shift(tag->symbol[part_idx]);
    return hash;
}

bool type_tag_eq(const type_tag *tag_a, const type_tag *tag_b) {
    if (tag_a == tag_b)
        return true;
    if (!tag_a || !tag_b)
        return false;
    for (size_t part_idx = 0; part_idx < TYPE_SYMBOL_PARTS; part_idx++)
        if (tag_a->symbol[part_idx] != tag_b->symbol[part_idx])
            return false;
    return tag_a->fn_table->eq_fn(tag_a->data, tag_b->data) && type_eq(tag_a->inner_type, tag_b->inner_type);
}

void type_tag_print(const type_tag *tag, FILE *file, int32_t idnt, type_print_opts opts) {
    fprintf(file, "%*s", idnt, "");
    fprintf(file, COLOR2(BOLD, WHITE) "[" COLOR(RESET));
    if (tag->inner_type)
        type_print(tag->inner_type, file, 0, TYPE_PRINT(_));
    uint8_t buffer[TYPE_SYMBOL_C_STR_SIZE] = {};
    if (type_tag_symbol_to_byte_array(tag, buffer) == DEF_STATUS(OK))
        printf(COLOR(CYAN) "`%s" COLOR(RESET), buffer);
    tag->fn_table->print_fn(tag->data, file, 1, tag->print_opts);
    fprintf(file, COLOR2(BOLD, WHITE) "]" COLOR(RESET));
    if (opts & TYPE_PRINT(CLASS_NL_END))
        fprintf(file, "\n");
}
