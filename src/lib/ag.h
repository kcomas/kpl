
#pragma once

#include "ast.h"
#include "gen.h"

typedef struct {
    uint32_t lc, tc; // label counter, temporary counter
    ssize_t r;
    gen *g;
    tbl *at, *ot; // not op, op
} ag; // ast to gen
