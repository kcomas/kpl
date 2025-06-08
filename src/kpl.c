
#include "kpl.h"

static const char *const tgs[] = {
    "STMT",
    "ARGS",
    "OP",
    "IF",
    "COND",
    "LOP",
    "VAR",
    "CODE",
    "MOD",
    "INT",
    "FLT",
    "STR",
    "VD",
    "BL",
    "U3",
    "U4",
    "U5",
    "U6",
    "I3",
    "I4",
    "I5",
    "I6",
    "F5",
    "F6",
    "DT",
    "CR",
    "SL",
    "SG",
    "VR",
    "TE",
    "HH",
    "ST",
    "FN",
    "ER",
    "FD"
};

const char *type_get_str(type t) {
    const char *s = "INVALID";
    if (t >= TYPE(STMT) && t <= TYPE(FD)) s = tgs[t];
    return s;
}
