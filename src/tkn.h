
#pragma once

#include "kpl.h"

#define TKN_STAT(N) TKN_STAT_##N

typedef enum {
    TKN_STAT(OK),
    TKN_STAT(END) // no more tkns
} tkn_stat;

typedef struct {
    size_t lno, cno, pos; // line, char, pos to get next tkn
} tkn_st;

inline void tkn_st_init(tkn_st *const ts) {
    ts->lno = ts->cno = 1;
    ts->pos = 0;
}

#define TKN_TYPE(N) TKN_TYPE##N

typedef enum {
    TKN_TYPE(CMT), // comment
    TKN_TYPE(VAR),
    TKN_TYPE(INT)
} tkn_type;

typedef struct {
    tkn_type type;
    size_t lno, cno, pos, len; // line, char, start pos, len
} tkn;

// state, token, str, inc state
tkn_stat _tkn_get(tkn_st *const ts, tkn *const t, const char *const str, bool inc);

inline tkn_stat tkn_next(tkn_st *const ts, tkn *const t, const char *const str) {
    return _tkn_get(ts, t, str, true);
}

inline tkn_stat tkn_peek(tkn_st *const ts, tkn *const t, const char *const str) {
    return _tkn_get(ts, t, str, false);
}
