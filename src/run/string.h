
#pragma once

#include "./header.h"

typedef struct _string {
    MEM_HEADER(_string);
    uint32_t len;
    char data[];
} string;

#ifndef STRING_MIN_SIZE
#define STRING_MIN_SIZE 100
#endif

string *string_init(uint32_t size);

string *string_init_c_str(const char *c_str);

void string_free(string *st);

#define STRING_PRINT(NAME) STRING_PRINT_##NAME

typedef enum [[gnu::packed]] {
    STRING_PRINT(NL_END)    = 1 << 0,
    STRING_PRINT(_)         = 0
} string_print_opts;

void string_print(const string *st, FILE *file, int32_t idnt, string_print_opts opts);
