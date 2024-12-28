
#pragma once

#include <stdarg.h>
#include "vr.h"
#include "tkn.h"

#define PSR_STAT(N) PSR_STAT_##N

typedef enum {
    PSR_STAT(OK), // continue success
    PSR_STAT(INV), // exit error
    PSR_STAT(END) // exit success
} psr_stat;

typedef struct _psr psr;

// on match
typedef psr_stat psr_pf(psr *const p, te *const n);

// pt match te[tkn id;fn;tbl]

// node te[id;parent;left;right;te[tkn]]

#define PASER(N) PASER_##N

typedef enum {
    PASER(UN) = 0,
    PASER(USR) = 1
} paser;

typedef struct _psr {
    ssize_t r;
    alfn *pa;
    frfn *pf;
    tkn *tt; // token tree
    vr *ts; // token stack
    tbl *pt; // parse tree
} psr;

psr *psr_i(alfn *pa, frfn *pf, tkn *tt, vr *ts, tbl *pt);

// give zero for assigned id, llen ....token ids
size_t psr_a(psr *const p, size_t id, psr_pf *pf, size_t l, ...);

void psr_f(psr *p);
