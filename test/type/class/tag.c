
#include "../../test.h"

TEST(symbol_from_c_str) {
    const char *string_a = "abcdefghijklMNOPQRSTUVWXYZ_123456789";
    uint8_t buffer[TYPE_SYMBOL_C_STR_SIZE] = {};
    type_tag *tag = type_tag_init(NULL, 0, &def_unused_fn_table, DEF(_));
    ASSERT(type_tag_symbol_from_c_str_slice(tag, string_a, strlen(string_a)) == DEF_STATUS(OK), "invalid symbol");
    ASSERT(type_tag_symbol_to_byte_array(tag, buffer) == DEF_STATUS(OK), "invalid symbol to buffer");
    printf(COLOR(CYAN) "`%s\n" COLOR(RESET), buffer);
    ASSERT(!strcmp(string_a, (char*) buffer), "invalid symbol string");
    memset(buffer, '\0', TYPE_SYMBOL_C_STR_SIZE);
    type_tag_free(tag);
}
