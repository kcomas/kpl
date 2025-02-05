
#pragma once

#include "def.h"
#include "mc.h"
#include "tbl.h"

#define TKN_STAT(N) TKN_STAT_##N

typedef enum {
    TKN_STAT(OK), // found tkn
    TKN_STAT(INV),
    TKN_STAT(END) // end of str
} tkn_stat;

typedef tbl *tkn_tbl_i(void);

typedef struct _tkn tkn;

// on match
typedef tkn_stat tkn_pf(tkn *t, te *m);

// match te[id;lno;cno;start;end]

#define TOKEN(N) TOKEN_##N

typedef enum {
    TOKEN(UN) = 0, // unknown
    TOKEN(_),
    TOKEN(USR) = 1000
} token;

typedef struct _tkn {
    ssize_t r;
    size_t idc, lno, cno, pos; // id counter
    const alfr *af, *ta;
    frfn *ef; // entry free
    tkn_tbl_i *ttif;
    tkn_pf *df; // called when nothing matches
    tbl *t; // trie
    mc *s;
} tkn;

tkn *tkn_i(const alfr *af, const alfr *ta, frfn *ef, tkn_tbl_i *ttif, tkn_pf *df, mc *s);

// give zero for an assigned id
size_t tkn_a(tkn *t, size_t tid, const char *s, tkn_pf *pf);

tkn_stat tkn_n(tkn *t, te *m);

void tkn_s(tkn *t, te *m);

void tkn_f(tkn *t);
