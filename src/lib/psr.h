
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
typedef psr_stat psr_pf(psr *const p, te **const n);

// pt match te[tkn id;fn;tbl]

// node te[id;data;parent;left;right;te[tkn]]

#define PARSER(N) PARSER_##N

typedef enum {
    PARSER(UN) = 0,
    PARSER(USR)
} parser;

typedef tbl* psr_tbl_i(void);

typedef struct _psr {
    ssize_t r;
    size_t idc;
    alfn *pa;
    frfn *pf, *ef; // entry free
    psr_tbl_i *prti;
    psr_pf *df; // no match
    tkn *tt; // token tree
    vr *ts; // token stack
    tbl *pt; // parse tree
} psr;

psr *psr_i(alfn *pa, frfn *pf, frfn *ef, psr_tbl_i *prti, psr_pf *df, tkn *tt, vr *ts);

// give zero for assigned id, llen ....token ids
size_t psr_a(psr *const p, size_t id, psr_pf *pf, size_t l, ...);

void psr_f(psr *p);
