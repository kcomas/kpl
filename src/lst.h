
#pragma once

#include "def.h"

typedef struct _lst_itm {
    struct _lst_itm *p, *n;
    un d;
} lst_itm;

// linked list
typedef struct {
    ssize_t r; // ref count
    size_t l; // len
    alfn *la, *ta; // list allocator, itm allocator
    frfn *tf, *df, *lf; // itm free, data free, lst free
    lst_itm *h, *t; // head, tail
} lst;

lst *lst_i(alfn *la, alfn *ta, frfn *tf, frfn *df, frfn *lf);

lst *lst_c(lst *l);

size_t lst_g_l(const lst *const l);

void lst_f(lst *l);
