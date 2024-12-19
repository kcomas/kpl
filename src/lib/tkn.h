
#pragma once

#include "def.h"
#include "tbl.h"

#define TKN_STAT(N) TKN_STAT_##N

typedef enum {
    TKN_STAT(OK),
    TKN_STAT(END)
} tkn_stat;

typedef struct {
    size_t lno, cno, pos, ml; // line, char, pos, max tkn len
    const char *str;
    tbl *t;
} tkn_st;

void tkn_st_i(tkn_st *const ts, const char *str, tbl *t);

void tkn_st_n(tkn_st *const ts, bool incl);

typedef struct _tkn_d tkn_d;

typedef tkn_stat tkn_g(tkn_st *const ts, const te *const tkn, size_t *const pos);

// tkn te[sg;tkn_d]
typedef struct _tkn_d {
    size_t id;
    ssize_t l; // -1 for variable
    tkn_g *pf; // parse
} tkn_d;

void tkn_st_f(tkn_st *ts);
