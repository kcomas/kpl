
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
    TYPE(C4),
    TYPE(CS),
    TYPE(SG),
    // vector
    TYPE(_V),
    TYPE(SL),
    TYPE(VR),
    TYPE(LT),
    TYPE(MC),
    TYPE(ER),
    TYPE(CJ),
    // hash
    TYPE(_H),
    TYPE(ST),
    TYPE(ET),
    TYPE(UN),
    // function
    TYPE(_F),
    TYPE(FN),
    TYPE(NF),
    TYPE(FP),
    // collection
    TYPE(_C),
    TYPE(TE),
    TYPE(KV),
    TYPE(BA),
    TYPE(TD),
    TYPE(_END)
} type;

const char *type_str(type t);

// check if they type is passed as pointer
bool type_is_ref(type t);

// check if type needs a destructor fn
bool type_is_des(type t);

#define TYPE_CLS(N) TYPE_CLS_##N

// parent type is weak ref
typedef enum {
    TYPE_CLS(S), // te[parent;type]
    TYPE_CLS(V), // te[parent;type;te[...]]
    TYPE_CLS(H), // te[parent;type;tbl[mc;_;type]]
    TYPE_CLS(F), // te[parent;type;te[...];tbl[mc;id;type];tbl[mc;id;type]]
    TYPE_CLS(C), // te[parent;type;...]
    TYPE_CLS(_)
} type_cls; // how type is stored

type_cls type_g_c(type t);

// type tbl entry te[mc;id;type]
void type_tbl_a(tbl *t, const alfr *af, mc *s, uint32_t id, te *type);

te *type_s_i(const alfr *af, te *p, type t);

te *type_v_i(const alfr *af, te *restrict p, type v, te *restrict t);

te *type_h_i(const alfr *af, te *restrict p, type h, tbl *restrict t);

ssize_t type_h_cmp(un a, un b);

te *type_f_i(const alfr *af, te *restrict p, type f, te *restrict r, tbl *restrict a, tbl *restrict s);

te *type_i(const alfr *af, te *restrict p, type t);

te *type_te_i(const alfr *af, te *restrict p, size_t l);

te *type_te_i_v(const alfr *af, te *restrict p, size_t l, ...);

te *type_te_g_t(const te *t, size_t n); // zero indexed

bool type_te_eq(const te *t); // if all types in te are the same

void type_p(const te *t);

bool type_eq(const te *restrict a, const te *restrict b);

bool type_un_eq(un a, un b);

// check if b exists on on a parents
bool type_ic(const te *restrict a, const te *restrict b);

size_t type_hsh(const te *t);

size_t type_un_hsh(un v);

bool type_has_refs(const te *t);

te *type_cpy(const te *t);
