
#pragma once

#include "../../run/run.h"

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

typedef struct _type type;

typedef struct _type_tag {
    MEM_HEADER(_type_tag);
    type *inner_type;
    def_fn_table *fn_table;
    def_data data;
    size_t id;
    uint64_t symbol[TYPE_SYMBOL_PARTS];
} type_tag;

def_status type_tag_symbol_from_c_str(type_tag *tag, const char *c_str);

def_status type_tag_symbol_to_c_str(const type_tag *tag, char c_str[TYPE_SYMBOL_C_STR_SIZE]);
