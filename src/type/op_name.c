
#include "./op_name.h"

static const char *op_name_strs[] = {
    "",
    "INDEX",
    "INVALID"
};

const char *type_op_name_str(type_op_name op_name) {
    if (op_name >= TYPE_OP_NAME(INVALID))
        return "INVALID OP";
    return op_name_strs[op_name];
}
