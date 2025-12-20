
#include "./string.h"

MEM_POOL(string_pool);

constexpr uint32_t string_max_size = UINT32_MAX - sizeof(string) - sizeof(char);

string *string_init(uint32_t size) {
    if (size < STRING_MIN_SIZE)
        size = STRING_MIN_SIZE;
    if (size > string_max_size)
        return NULL;
    string *st = mem_alloc(&string_pool, sizeof(string) + size * sizeof(char) + sizeof(char));
    st->len = 0;
    memset(st->data, '\0', size * sizeof(char) + sizeof(char));
    return st;
}

string *string_init_c_str(const char *c_str) {
    uint32_t str_len = strlen(c_str);
    string *st = string_init(str_len);
    st->len = str_len;
    strcpy(st->data, c_str);
    return st;
}

void string_free(string *st) {
    mem_free(&string_pool, st);
}

void string_print(const string *st, FILE *file, int32_t idnt, string_print_opts opts) {
    fprintf(file, "%*s", idnt, "");
    fprintf(file, COLOR(LIGHT_GREEN) "%s" COLOR(RESET), st->data);
    if (opts & STRING_PRINT(NL_END))
        fprintf(file, "\n");
}
