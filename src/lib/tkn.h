
#pragma once

#include "def.h"
#include "mc.h"
#include "tbl.h"

#define TKN_STAT(N) TKN_STAT_##N

typedef enum {
    TKN_STAT(OK),
    TKN_STAT(NF),
    TKN_STAT(INV),
    TKN_STAT(END)
} tkn_stat;

typedef tbl *tkn_tbl_i(void);

typedef struct _tkn tkn;

// on match
typedef tkn_stat tkn_pf(tkn *const t, te *const m);

// match te[id;lno;cno;start;end]

typedef struct _tkn {
    ssize_t r;
    size_t lno, cno, pos;
    alfn *ta;
    frfn *tf, *ef; // entry free
    tkn_tbl_i *ttif;
    tkn_pf *df; // called when nothing matches
    tbl *t; // trie
    mc *s;
} tkn;

tkn *tkn_i(alfn *ta, frfn *tf, frfn *ef, tkn_tbl_i *ttif, tkn_pf *df, mc *s);

void tkn_a(tkn *const t, const char *const s, ssize_t id, tkn_pf *pf);

tkn_stat tkn_n(tkn *const t, te *const m);

void tkn_s(tkn *const t, te *const m);

void tkn_f(tkn *t);
