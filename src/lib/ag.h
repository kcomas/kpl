
#pragma once

#include "ast.h"
#include "gen.h"

typedef struct {
    uint32_t lc; // label counter
    ssize_t r;
    gen *g; // for base gens
    as *a;
    tbl *at, *ot; // not op, op
} ag; // ast to gen
