
#include "tbl.h"

tbl *tbl_i(alfn *at, frfn *ft, hhfn *hf, cmpfn *cf, lst *i, te *b) {
    tbl *t = alfn(sizeof(tbl));
    t->r = 1;
    t->u = 0;
    t->hf = hf;
    t->cf = cf;
    t->i = i;
    t->b = b;
    return t;
}
