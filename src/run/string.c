
#include "./string.h"

MEM_POOL(string_pool);

constexpr uint32_t string_max_size = UINT32_MAX - sizeof(string) - sizeof(char);

string *string_init(uint32_t size) {
    if (size < STRING_MIN_SIZE)
        size = STRING_MIN_SIZE;
    if (size > string_max_size)
        return nullptr;
    string *str = mem_alloc(&string_pool, sizeof(string) + size * sizeof(char) + sizeof(char));
    str->len = 0;
    memset(str->data, '\0', size * sizeof(char) + sizeof(char));
    return str;
}

string *string_init_c_str(const char *c_str) {
    uint32_t str_len = strlen(c_str);
    string *str = string_init(str_len);
    str->len = str_len;
    strcpy(str->data, c_str);
    return str;
}

string *string_init_c_str_slice(const char *c_str, uint32_t len) {
    string *str = string_init(len);
    str->len = len;
    memcpy(str->data, c_str, len);
    return str;
}

void string_free(string *str) {
    mem_free(&string_pool, str);
}

size_t string_hash(const string *str) {
    size_t hash = 5831;
    const char *data = str->data;
    while (*data)
        hash = ((hash << 5) + hash) + *data++;
    return hash;
}

ssize_t string_cmp(const string *st_a, const string *st_b) {
    return strcmp(st_a->data,  st_b->data);
}

bool string_eq(const string *str_a, const string *str_b) {
    if (str_a == str_b)
        return true;
    if (!str_a || !str_b || str_a->len != str_b->len)
        return false;
    return !strcmp(str_a->data, str_b->data);
}

string *string_copy(const string *str) {
    string *str_copy = string_init(str->len);
    str_copy->len = str->len;
    memcpy(str_copy->data, str->data, str->len);
    return str_copy;
}

void string_print(const string *str, FILE *file, int32_t idnt, string_print_opts print_opts) {
    if (print_opts & STRING_PRINT(NL_START))
        fprintf(file, "\n");
    fprintf(file, "%*s", idnt, "");
    if (print_opts & STRING_PRINT(ESCAPE)) {
        fprintf(file, COLOR(GREEN) "\"");
        for (uint32_t str_idx = 0; str_idx < str->len; str_idx++) {
            char character = str->data[str_idx];
            switch (character) {
                case '\0':
                    fprintf(file, "\\0");
                    break;
                case '\t':
                    fprintf(file, "\\t");
                    break;
                case '\n':
                    fprintf(file, "\\n");
                    break;
                default:
                    fprintf(file, "%c", character);
                    break;
            }
        }
        fprintf(file, "\"" COLOR(RESET));
    } else
        fprintf(file, COLOR(GREEN) "\"%s\"" COLOR(RESET), str->data);
    if (print_opts & STRING_PRINT(NL_END))
        fprintf(file, "\n");
}

def_fn_table string_fn_table = {
    .hash_fn = (void*) string_hash,
    .cmp_fn = (void*) string_cmp,
    .eq_fn = (void*) string_eq,
    .copy_fn = (void*) string_copy,
    .serialize_fn = nullptr,
    .print_fn = (void*) string_print,
    .free_fn = (void*) string_free
};
