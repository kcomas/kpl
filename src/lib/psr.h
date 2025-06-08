
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

#define PSR_MODE(N) PSR_MODE_##N

typedef enum {
    PSR_MODE(NONE), // no mode
    PSR_MODE(ONCE), // keep going if no stop tkns
    PSR_MODE(LOOP) // until stop tkns
} psr_mode;

#define PARSER(N) PARSER_##N

typedef enum {
    PARSER(UN) = 0,
    PARSER(_),
    PARSER(USR) = 1000
} parser;

typedef struct _psr psr;

typedef tbl *psr_tbl_i(void);

// m tkn match
typedef psr_stat psr_node_fn(psr *const p, te **n);

// head node, current node, next node ok to continue end to stop
typedef psr_stat psr_megre_fn(psr *const p, te **h, void ***c, te *const n);

// psr entry te[tkn_id;psr_id;mode;te[stop_tkns];merge_fn;node_fn(null for none);tbl]

typedef struct _psr {
    ssize_t r;
    size_t idc;
    alfn *pa;
    frfn *pf, *pef;
    psr_tbl_i *pti;
    tkn *tt;
    vr *ts; // tkn stk
    tbl *pt;
} psr;

psr *psr_i(alfn *pa, frfn *pf, frfn *pef, psr_tbl_i *pti, tkn *tt, vr *ts);

// returns 0 for insert fail
size_t psr_a(psr *const p, size_t pid, size_t mode, te *const st, psr_megre_fn *mf, psr_node_fn *nf, size_t nt, ...);

// start tkn_id node zero for none
psr_stat psr_n(psr *const p, te **h);

void psr_f(psr *p);
