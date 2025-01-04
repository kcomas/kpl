
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

#define PARSER(N) PARSER_##N

typedef enum {
    PARSER(UN) = 0,
    PARSER(_),
    PARSER(USR) = 1000
} parser;

typedef struct _psr psr;

typedef tbl *psr_tbl_i(void);

// m tkn match
typedef te *psr_node_fn(psr *const p, const te *const m);

// current node, next node
typedef psr_stat psr_megre_fn(psr *const p, te **c, te *const n);

// psr entry te[tkn_id;psr_id;te[stop tkn id](null for tknend);merge_fn;node_fn(null for none);tbl]

typedef struct _psr {
    ssize_t r;
    alfn *pa;
    frfn *pf, *pef;
    psr_tbl_i *pti;
    tkn *tt;
    tbl *pt;
} psr;

psr *psr_i(alfn *pa, frfn *pf, frfn *pef, psr_tbl_i *pti, tkn *tt);

// returns 0 for insert fail
size_t psr_a(psr *const p, size_t id, te *st, psr_megre_fn *mf, psr_node_fn *nf, size_t nt, ...);

void psr_f(psr *p);
