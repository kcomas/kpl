
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
    return NULL;
}
