
#include "chk.h"

void chk_f(chk *c) {
    if (!c || --c->r > 0) return;
    tbl_f(c->ct);
    c->af->f(c);
}
