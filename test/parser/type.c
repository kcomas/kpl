
#include "../test.h"

TEST(parser_type) {
    string *str = string_init_c_str("Byte_vector : Vector[U8]");
    token_state state;
    token_state_init(&state, str);
    token_slice slice;
    ASSERT(token_next(&state, &slice) == DEF_STATUS(OK), "invalid token next");
    token_slice_print(&slice, stdout, 0, TOKEN_SLICE_PRINT(NL_END));
    type *ty = parser_type(&slice);
    type_print(ty, stdout, 0, TYPE_PRINT(NL_END));
    ASSERT(ty && ty->name == TYPE_NAME(ALIAS), "invalid token type");
    type_free(ty);
    ASSERT(token_next(&state, &slice) == DEF_STATUS(OK), "invalid token next");
    ASSERT(token_next(&state, &slice) == DEF_STATUS(OK), "invalid token next");
    ASSERT(token_next(&state, &slice) == DEF_STATUS(OK), "invalid token next");
    ASSERT(token_next(&state, &slice) == DEF_STATUS(OK), "invalid token next");
    token_slice_print(&slice, stdout, 0, TOKEN_SLICE_PRINT(NL_END));
    ty = parser_type(&slice);
    type_print(ty, stdout, 0, TYPE_PRINT(NL_END));
    ASSERT(ty && ty->name == TYPE_NAME(VECTOR), "invalid token type");
    type_free(ty);
    ASSERT(token_next(&state, &slice) == DEF_STATUS(OK), "invalid token next");
    ASSERT(token_next(&state, &slice) == DEF_STATUS(OK), "invalid token next");
    ty = parser_type(&slice);
    type_print(ty, stdout, 0, TYPE_PRINT(NL_END));
    ASSERT(ty && ty->name == TYPE_NAME(U8), "invalid token type");
    type_free(ty);
    string_free(str);
}
