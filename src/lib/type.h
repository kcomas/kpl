
#pragma once

#include "te.h"

#define TYPE(N) TYPE_##N

typedef enum {
    TYPE(VD),
    TYPE(U3),
    TYPE(U4),
    TYPE(U5),
    TYPE(U6),
    TYPE(I3),
    TYPE(I4),
    TYPE(I5),
    TYPE(I6),
    TYPE(C4),
    TYPE(SG),
    TYPE(TE),
    TYPE(ST),
    TYPE(LL),
    TYPE(VR),
    TYPE(HH),
    TYPE(FN),
    TYPE(FD),
    TYPE(TD)
} type_id;

typedef struct {


} type;
