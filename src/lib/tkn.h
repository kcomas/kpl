
#pragma once

#include "def.h"
#include "mc.h"
#include "tbl.h"

#define TKN_STAT(N) TKN_STAT_##N

typedef enum {
    TKN_STAT(OK),
    TKN_STAT(INV),
    TKN_STAT(END)
} tkn_stat;

typedef tbl *tkn_tbl_i(void);

typedef struct _tkn tkn;

// on match
typedef tkn_stat tkn_pf(tkn *const t, te *const m);

// match te[id;lno;cno;start;end]

#define TOKEN(N) TOKEN_##N

typedef enum {
    TOKEN(UN) = 0, // unknown
    TOKEN(VAR) = 1,
    TOKEN(NUM) = 2,
    TOKEN(USR) = 3
} token;

typedef struct _tkn {
    ssize_t r;
    size_t idc, lno, cno, pos; // id counter
    alfn *ta;
    frfn *tf, *ef; // entry free
    tkn_tbl_i *ttif;
    tkn_pf *df; // called when nothing matches
    tbl *t; // trie
    mc *s;
} tkn;

tkn *tkn_i(alfn *ta, frfn *tf, frfn *ef, tkn_tbl_i *ttif, tkn_pf *df, mc *s);

// give zero for an assigned id
size_t tkn_a(tkn *const t, size_t id, const char *const s, tkn_pf *pf);

tkn_stat tkn_n(tkn *const t, te *const m);

void tkn_s(tkn *const t, te *const m);

void tkn_f(tkn *t);
