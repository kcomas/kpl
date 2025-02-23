
#pragma once

#include <stdio.h>
#include "./lib/tbl.h"

#define TYPE(N) TYPE_##N

typedef enum {
    TYPE(_START),
    // scalar
    TYPE(VD), // void
    TYPE(BL),
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
    TYPE(SL),
    TYPE(C4),
    TYPE(SG),
    // vector
    TYPE(VR),
    TYPE(LT),
    // hash
    TYPE(HH),
    TYPE(ST),
    // function
    TYPE(FN),
    TYPE(SF),
    TYPE(CF),
    // collection
    TYPE(TE),
    TYPE(BA),
    TYPE(TD),
    TYPE(UN),
    TYPE(_END)
} type;

const char *type_str(type t);

#define TYPE_CLS(N) TYPE_CLS_##N

typedef enum {
    TYPE_CLS(S), // te[type]
    TYPE_CLS(V), // te[type;te[...]]
    TYPE_CLS(H), // te[type;tbl[mc;type]]
    TYPE_CLS(F), // te[type;tbl[mc;type];te[...]]
    TYPE_CLS(C), // te[type;...]
    TYPE_CLS(_)
} type_cls; // how type is stored

type_cls type_g_c(type t);

void type_tbl_a(tbl *t, const alfr *af, mc *s, size_t id, te *type);

te *type_s_i(const alfr *af, type t);

te *type_v_i(const alfr *af, type v, te *t);

te *type_h_i(const alfr *af, type h, tbl *t);

te *type_f_i(const alfr *af, type f, te *r, tbl *a);

te *type_i(const alfr *af, type t);

void type_p(const te *t);

bool type_eq(const te *restrict a, const te *restrict b);
