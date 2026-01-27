
#include "./qualifier_flags.h"

const char *type_qualifier_str(type_qualifier_flags qualifier_flags) {
    switch (qualifier_flags & TYPE_QUALIFIER_MASK) {
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

const char *type_flag_str(type_qualifier_flags qualifier_flags) {
    switch (qualifier_flags & TYPE_FLAG_MASK) {
        case TYPE_FLAG(TRACING_GC):
            return "TRACING_GC";
        case TYPE_FLAG(MOVED):
            return "MOVED";
        case TYPE_FLAG(REGISTER):
            return "REGISTER";
        case TYPE_FLAG(C_CODE):
            return "C_CODE";
        default:
            break;
    }
    return nullptr;
}
