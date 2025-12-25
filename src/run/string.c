
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

size_t string_hash(const string *st) {
    size_t hash = 5831;
    const char *data = st->data;
    while (*data)
        hash = ((hash << 5) + hash) + *data++;
    return hash;
}

ssize_t string_cmp(const string *st_a, const string *st_b) {
    return strcmp(st_a->data,  st_b->data);
}

bool string_eq(const string *st_a, const string *st_b) {
    if (st_a == st_b)
        return true;
    if (!st_a || !st_b || st_a->len != st_b->len)
        return false;
    return !strcmp(st_a->data, st_b->data);
}

string *string_copy(const string *st) {
    string *st_copy = string_init(st->len);
    st_copy->len = st->len;
    memcpy(st_copy->data, st->data, st->len);
    return st_copy;
}

void string_print(const string *st, FILE *file, int32_t idnt, string_print_opts print_opts) {
    if (print_opts & STRING_PRINT(NL_START))
        fprintf(file, "\n");
    fprintf(file, "%*s", idnt, "");
    fprintf(file, COLOR(GREEN) "\"%s\"" COLOR(RESET), st->data);
    if (print_opts & STRING_PRINT(NL_END))
        fprintf(file, "\n");
}

def_fn_table string_fn_table = {
    .hash_fn = (void*) string_hash,
    .cmp_fn = (void*) string_cmp,
    .eq_fn = (void*) string_eq,
    .copy_fn = (void*) string_copy,
    .serialize_fn = NULL,
    .print_fn = (void*) string_print,
    .free_fn = (void*) string_free
};
