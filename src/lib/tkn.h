
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

typedef struct {
    size_t id, lno, cno, pos;
} tkn_m;

typedef tkn_stat tkn_pf(tkn *const t, tkn_m *const m);

typedef struct _tkn {
    ssize_t r;
    size_t lno, cno, pos;
    alfn *ta;
    frfn *tf;
    tkn_tbl_i *ttif;
    tkn_pf *df; // called when nothing matches
    tbl *t; // trie
    mc *s;
} tkn;

tkn *tkn_i(alfn *ta, frfn *tf, tkn_tbl_i *ttif, tkn_pf *df, mc *s);

void tkn_a(tkn *const t, const char *const s, size_t id, tkn_pf *pf);

void tkn_f(tkn *t);
