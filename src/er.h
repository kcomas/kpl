
#pragma once

#include "kpl.h"

#define ER(N) ER_##N

typedef enum {
    ER(MOD),
    ER(TKN),
    ER(AST),
    ER(TYPE),
    ER(CODE),
    ER(JIT),
    ER(RUN)
} er_type;

typedef union {
    char *mod;
} er_data;

typedef struct _er_itm {
    er_type et;
    const char *stat;
    struct _er_itm *prev, *next;
    er_data *ed;
} er_itm;

inline er_itm *er_itm_i(er_type et, const char *stat, er_data *ed) {
    er_itm *ei = calloc(1, sizeof(er_itm);
    ei->et = et;
    ei->stat = stat;
    ei->ed = ed;
    return ei;
}

typedef struct {
    size_t len;
    er_itm *h, *t;
} er;

inline er *er_i(void) {
    return calloc(1, sizeof(er));
}

inline void er_a(er *const e, er_itm *const ei) {
    LST_A(e, ei);
}

inline void er_f(er *e) {
    // TODO
}
