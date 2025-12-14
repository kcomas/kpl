
#include "./class_qualifier_flags.h"

static const char *class_str_array[] = {
    "SCALAR",
    "ARRAY",
    "VECOR",
    "OP",
    "ENUM",
    "STRUCT",
    "FN",
    "LAMBDA",
    "OVERLOAD",
    "TAG",
    "ALIAS",
    "VAR",
    "_"
};

const char *type_class_str(type_class class) {
    if (class >= TYPE_CLASS(_))
        return "INVALID TYPE CLASS";
    return class_str_array[class];
}

const char *type_qualifier_str(type_qualifier_flags qualifier_flags) {
    switch (qualifier_flags & TYPE_QUALIFIER_MASK) {
        case TYPE_QUALIFIER(CONST):
            return "CONST";
        case TYPE_QUALIFIER(REF):
            return "REF";
        case TYPE_QUALIFIER(SHARED):
            return "SHARED";
        case TYPE_QUALIFIER(NAMESPACE):
            return "NAMESPACE";
        case TYPE_QUALIFIER(MUTEX):
            return "MUTEX";
        default:
            break;
    }
    return "INVALID TYPE QUALIFIER";
}
