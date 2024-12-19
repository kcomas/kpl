
#pragma once

#include "def.h"

#define TKN_STAT(N) TKN_STAT_##N

typedef enum {
    TKN_STAT(OK),
    TKN_STAT(NF),
    TKN_STAT(INV),
    TKN_STAT(END)
} tkn_stat;

