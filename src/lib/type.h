
#pragma once

#include <stdio.h>
#include "tbl.h"

#define TYPE(N) TYPE_##N

typedef enum {
    TYPE(_START),
    // scalar
    TYPE(_S),
    // internal
    TYPE(_N), // none
    TYPE(_A), // auto
    TYPE(_L), // deferred list {}
    TYPE(_G), // gen code
    // user
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
    TYPE(_V),
    TYPE(VR),
    TYPE(LT),
    TYPE(MC),
    // hash
    TYPE(_H),
    TYPE(UN),
    TYPE(HH),
    TYPE(ST),
    // function
    TYPE(_F),
    TYPE(FN),
    TYPE(SF),
    TYPE(CF),
    // collection
    TYPE(_C),
    TYPE(TE),
    TYPE(BA),
    TYPE(TD),
    TYPE(_END)
} type;

const char *type_str(type t);

#define TYPE_CLS(N) TYPE_CLS_##N

// parent type is weak ref
typedef enum {
    TYPE_CLS(S), // te[parent;type]
    TYPE_CLS(V), // te[parent;type;te[...]]
    TYPE_CLS(H), // te[parent;type;tbl[mc;id;type]]
    TYPE_CLS(F), // te[parent;type;te[...];tbl[mc;id;type]]
    TYPE_CLS(C), // te[parent;type;...]
    TYPE_CLS(_)
} type_cls; // how type is stored

type_cls type_g_c(type t);

// type tbl entry te[mc;id;type]
void type_tbl_a(tbl *t, const alfr *af, mc *s, size_t id, te *type);

te *type_s_i(const alfr *af, te *p, type t);

te *type_v_i(const alfr *af, te *restrict p, type v, te *restrict t);

te *type_h_i(const alfr *af, te *restrict p, type h, tbl *restrict t);

te *type_f_i(const alfr *af, te *restrict p, type f, te *restrict r, tbl *a);

te *type_i(const alfr *af, te *restrict p, type t);

void type_p(const te *t);

bool type_eq(const te *restrict a, const te *restrict b);

// check if b exists on on a parents
bool type_ic(const te *restrict a, const te *restrict b);
