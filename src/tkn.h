
#pragma once

#include "kpl.h"

#define TKN_STAT(N) TKN_STAT_##N

typedef enum {
    TKN_STAT(OK)
} tkn_stat;

typedef struct {
    size_t lno, cno, pos; // line, char, pos to get next tkn
} tkn_st;

inline void tkn_st_init(tkn_st *t) {
    t->lno = t->cno = 1;
    t->pos = 0;
}

#define TKN_TYPE(N) TKN_TYPE##N

typedef enum {

} tkn_type;

typedef struct {
    tkn_type type;
    size_t lno, cno, spos, len; // line, char, start pos, len
} tkn;
