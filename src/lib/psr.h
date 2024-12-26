
#pragma once

#include "vr.h"
#include "tkn.h"

// pt match te[tkn id;fn;tbl]

typedef struct {
    ssize_t r;
    alfn *pa;
    frfn *pf;
    tkn *tt; // token tree
    vr *ts; // token stack
    tbl *pt; // parse tree
} psr;
