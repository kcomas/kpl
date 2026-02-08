
#include "./qualifier_flags.h"

extern inline uint32_t type_qualifier_mask(void);

const char *type_qualifier_str(type_qualifier_flags qualifier_flags) {
    switch (qualifier_flags & type_qualifier_mask()) {
        case TYPE_QUALIFIER(CONST):
            return "CONST";
        case TYPE_QUALIFIER(REF):
            return "REF";
        case TYPE_QUALIFIER(SHARED):
            return "SHARED";
        case TYPE_QUALIFIER(MUTEX):
            return "MUTEX";
        default:
            break;
    }
    return nullptr;
}

extern inline uint32_t type_flag_mask(void);

const char *type_flag_str(type_qualifier_flags qualifier_flags) {
    switch (qualifier_flags & type_flag_mask()) {
        case TYPE_FLAG(TRACING_GC):
            return "TRACING_GC";
        case TYPE_FLAG(MOVED):
            return "MOVED";
        case TYPE_FLAG(UNUSED):
            return "UNUSED";
        case TYPE_FLAG(REGISTER):
            return "REGISTER";
        case TYPE_FLAG(C_CODE):
            return "C_CODE";
        case TYPE_FLAG(PRINT_IR):
            return "PRINT_IR";
        case TYPE_FLAG(PRINT_AST):
            return "PRINT_AST";
        default:
            break;
    }
    return nullptr;
}
