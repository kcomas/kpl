
#include "as_t.h"

static size_t no_hsh(un d) {
    return d.u6;
}

static bool id_eq(un a, un b) {
    return a.u6 == b.u6;
}

lst *as_mklst(void) {
    return lst_i(&malloc, &malloc, &free, (void*) &te_f, &free);
}

tbl *as_mktbl(void) {
    lst *tl = as_mklst();
    te *b = te_i(10, &malloc, &free);
    tbl *t = tbl_i(&malloc, &free, &no_hsh, &id_eq, tl, b);
    return t;
}

void as_code_p(const as *const a) {
    te *h = a->code->h;
    while (h) {
        te *c = (te*) h->d[0].p;
        if (c->d[0].u6 == CODE_ID(L)) printf("%lu:\n", c->d[1].u6);
        else {

        }
        h = h->d[2].p;
    }
}

void label_entry_f(void *p) {
    te *l = (te*) p;
    te_f(l->d[1].p);
    lst_f(l->d[2].p);
    free(l);
}

void op_entry_f(void *p) {
    te *oe = (te*) p;
    tbl_f(oe->d[2].p);
    free(oe);
}

void code_entry_f(void *p) {
    te *o = (te*) p;
    te_f(o->d[2].p);
    te_f(o->d[3].p);
    te_f(o->d[4].p);
    te_f(o->d[5].p);
    free(o);
}
