
#include "../test.h"

bool token_match(const char *c_str, size_t class_array_len, token_class class_array[]) {
    string *str = string_init_c_str(c_str);
    string_print(str, stdout, 0, STRING_PRINT(ESCAPE) | STRING_PRINT(NL_END));
    token_state state;
    token_state_init(&state, str);
    size_t class_array_idx = 0;
    token_slice slice = { .class = TOKEN_CLASS(INVALID) };
    while (slice.class != TOKEN_CLASS(END) &&  class_array_idx < class_array_len) {
        if (token_next(&state, &slice) != DEF_STATUS(OK))
            break;
        token_slice_print(&slice, stdout, 0, TOKEN_SLICE_PRINT(CLASS) | TOKEN_SLICE_PRINT(NL_END));
        if (slice.class != class_array[class_array_idx++])
            break;
    }
    string_free(str);
    return class_array_idx == class_array_len;
}

#define C(NAME) TOKEN_CLASS(NAME)

TEST(token_basic) {
    const char *c_str = "a: 1 + 2\n";
    token_class class_array[] = { C(VAR), C(COLON), C(SPACES), C(INTEGER), C(SPACES), C(PLUS),
        C(SPACES), C(INTEGER), C(NEW_LINE), C(END) };
    ASSERT(token_match(c_str, DEF_STATIC_ARRAY_SIZE(class_array), class_array), "invalid tokens");
}
