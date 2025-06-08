
#pragma once

#include "kpl.h"

#define ER(N) ER_##N

typedef enum {
    ER(OK),
    ER(MOD),
    ER(TKN),
    ER(AST),
    ER(TYPE),
    ER(CODE),
    ER(JIT),
    ER(RUN)
} er_type;

typedef struct _er_itm {
    er_type et;
    struct _er_itm *prev, *next;
    size_t lno, cno;
    const char *stat, *path; // status code, file path
    var_sg *sg;
    char fnn[]; // function name
} er_itm;

inline er_itm *er_itm_i(al *const a, er_type et, const char *const fnn, const char *const stat) {
    er_itm *ei = ala(a, sizeof(er_itm) + strlen(fnn) * sizeof(char) + sizeof(char));
    ei->et = et;
    strcpy(ei->fnn, fnn);
    ei->stat = stat;
    return ei;
}

void er_itm_p(er_itm *const ei);

void er_itm_f(er_itm *ei, void *fn);

typedef struct _er {
    size_t len;
    er_itm *h, *t;
} er;

inline er *er_i(al *const a) {
    return ala(a, sizeof(er));
}

void er_a(er *const e, er_itm *const ei);

er_itm *er_g(er *const e);

inline void er_p(er *const e) {
    er_itm *h = e->h;
    while (h) {
        er_itm_p(h);
        h = h->next;
    }
}

void er_e(er *const e);

void er_c(er *const e);

inline void er_f(er *e) {
    er_c(e);
    alf(e);
}
