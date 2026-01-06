
#include "../../test.h"

TEST(symbol_from_c_str) {
    const char *string_a = "abcdefghijklMNOPQRSTUVWXYZ_123456789";
    uint8_t buffer[TYPE_SYMBOL_C_STR_SIZE] = {};
    type_tag *tag = type_tag_init(nullptr, 0, &def_unused_fn_table, DEF(_));
    ASSERT(type_tag_symbol_from_c_str_slice(tag, string_a, strlen(string_a)) == DEF_STATUS(OK), "invalid symbol");
    ASSERT(type_tag_symbol_to_byte_array(tag, buffer) == DEF_STATUS(OK), "invalid symbol to buffer");
    type_tag_print(tag, stdout, 0, TYPE_PRINT(CLASS_NL_END));
    ASSERT(!strcmp(string_a, (char*) buffer), "invalid symbol string");
    type_tag_free(tag);
}
