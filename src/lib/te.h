
#pragma once

#include "def.h"

// tuple
typedef struct {
    ssize_t r;
    size_t l; // len
    alfn *ta;
    frfn *tf;
    un d[];
} te;

te *te_i(size_t l, alfn *ta, frfn *tf);

te *te_c(te *t);

size_t te_g_l(const te *t);

void te_f(te *t);
