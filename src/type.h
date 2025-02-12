
#pragma once

#include "./lib/tbl.h"

#define TYPE(N) TYPE_##N

typedef enum {
    TYPE(VD), // void
    TYPE(I3),
    TYPE(I4),
    TYPE(I5),
    TYPE(I6),
    TYPE(U3),
    TYPE(U4),
    TYPE(U5),
    TYPE(U6),
    TYPE(F5),
    TYPE(F6),
    TYPE(C4),
    TYPE(SG),
    TYPE(TE),
    TYPE(VR),
    TYPE(LT),
    TYPE(ST),
    TYPE(HH)
} type;

te *type_s_i(type t); // scalar
