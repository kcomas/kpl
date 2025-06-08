
#pragma once

#include "def.h"
#include "te.h"

#define LST_STAT(N) LST_STAT_##N

typedef enum {
   LST_STAT(OK),
   LST_STAT(SUB)
} lst_stat;

// linked list
typedef struct {
    ssize_t r; // ref count
    size_t l; // len
    alfn *la, *ta; // list allocator, itm allocator
    frfn *tf, *df, *lf; // itm free, data free, lst free
    te *h, *t; // head, tail
} lst;

lst *lst_i(alfn *la, alfn *ta, frfn *tf, frfn *df, frfn *lf);

lst *lst_c(lst *l);

size_t lst_g_l(const lst *const l);

lst_stat lst_ab(lst *const l, un d);

lst_stat lst_af(lst *const l, un d);

lst_stat lst_sb(lst *const l, un *d);

lst_stat lst_sf(lst *const l, un *d);

void lst_li_r(lst *const l, te *li);

void lst_f(lst *l);
