
#pragma once

#include "kpl.h"

#define TKN_STAT(N) TKN_STAT_##N

typedef enum {
    TKN_STAT(OK)
} tkn_stat;

typedef struct {
    size_t lno, cno, pos; // line, char, abs pos
} tkn_st;

#define TKN_TYPE(N) TKN_TYPE##N

typedef enum {

} tkn_type;

typedef struct {
    tkn_type type;
    size_t lno, cno, pos, len;
} tkn;
