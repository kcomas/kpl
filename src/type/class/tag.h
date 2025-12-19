
#pragma once

#include "../header.h"

// Stores A-Za-z0-9_ (0 - 63) values in 6 bits each
// as c str symbol(without `) + null term
// symbols can be 40 values max
// upper 4 bits on each part is the length of each part

#define TYPE_SYMBOL_PARTS 4

#define TYPE_SYMBOL_BITS_PER_PART 64

#define TYPE_SYMBOL_BITS_PER_VALUE 6

#define TYPE_SYMBOL_VALUES_PER_PART (TYPE_SYMBOL_BITS_PER_PART / TYPE_SYMBOL_BITS_PER_VALUE)

#define TYPE_SYMBOL_LEN (TYPE_SYMBOL_VALUES_PER_PART * TYPE_SYMBOL_PARTS)

#define TYPE_SYMBOL_PAD_STR 1

#define TYPE_SYMBOL_C_STR_SIZE (TYPE_SYMBOL_LEN + TYPE_SYMBOL_PAD_STR)

typedef struct _type_tag {
    RUN_HEADER(_type_tag);
    def_data data;
    type *inner_type;
    ssize_t stk_reg_id; // -1 not used
    uint64_t symbol[TYPE_SYMBOL_PARTS];
} type_tag;

type_tag *type_tag_init(type *inner_type, uint32_t print_opts, def_fn_table *fn_table, def_data data);

void type_tag_free(type_tag *tag);

def_status type_tag_symbol_from_c_str_slice(type_tag *tag, const char *c_str, size_t str_len);

def_status type_tag_symbol_to_byte_array(const type_tag *tag, uint8_t buffer[TYPE_SYMBOL_C_STR_SIZE]);

size_t type_tag_hash(const type_tag *tag);

bool type_tag_eq(const type_tag *tag_a, const type_tag *tag_b);

void type_tag_print(const type_tag *tag, FILE *file, int32_t idnt, type_print_opts opts);
