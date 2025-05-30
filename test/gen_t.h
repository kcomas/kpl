
#pragma once

#include "../src/lib/gen_x64.h"
#include "as_t.h"

extern gen *bg;

lst *gen_mklst(void);

tbl *gen_cls_info_tbl(void);

tbl *gen_op_tbl(size_t bcks);

void gen_build(_tests *_t, gen *g, uint8_t *m);

#define BUILD(G, M) gen_build(_t, G, M); \
    E()

#define S(stmt) A((stmt) == GEN_STAT(OK), "gen_a")
