
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
    PSR_MODE(STOP),
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
typedef psr_stat psr_node_fn(psr *p, te **n, err **e);

// node holder, ok to continue end to stop
typedef psr_stat psr_megre_fn(psr *p, te *restrict nh, te *restrict n, err **e);

// node for each, next node
typedef psr_stat psr_each_fn(psr *p, te *restrict en, te *restrict n, err **e);

// psr entry te[tkn_id;psr_id;mode;te[stop_tkns];each_fn;merge_fn;node_fn(null for none);tbl]

typedef struct _psr {
    tkn_stat tstat;
    ssize_t r;
    size_t idc;
    const alfr *af, *ta, *la, *ea;
    frfn *pef;
    psr_tbl_i *pti;
    tkn *tt;
    vr *ts; // tkn stk
    tbl *pt;
} psr;

psr *psr_i(const alfr *af, const alfr *ta, const alfr *la, const alfr *ea, frfn pef, psr_tbl_i pti, tkn *tt, vr *ts);

psr *psr_i_psr(const psr *p, mc *s);

psr *psr_c(psr *p);

// returns 0 for insert fail
size_t psr_a(psr *p, size_t pid, size_t mode, te *st, psr_each_fn *ef, psr_megre_fn *mf, psr_node_fn *nf, size_t nt, ...);

psr_stat psr_err(psr *p, err **e, const char *m);

// node holder, tkn match
psr_stat psr_n(psr *p, te *nh, err **e);

void psr_f(psr *p);
