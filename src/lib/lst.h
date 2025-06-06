
#pragma once

#include <stdarg.h>
#include "def.h"
#include "te.h"

extern const alfr al_lst;

#define LST_STAT(N) LST_STAT_##N

typedef enum {
   LST_STAT(OK),
   LST_STAT(NF),
   LST_STAT(SUB)
} lst_stat;

typedef struct {
    ssize_t r; // ref count
    size_t l; // len
    const alfr *af, *ta;
    frfn *df; // data free
    te *h, *t; // head, tail
} lst; // linked list

lst *lst_i(const alfr *af, const alfr *ta, frfn *df);

lst *lst_i_v(const alfr *af, const alfr *ta, frfn *df, size_t n, ...);

lst *lst_i_lst(const lst *l);

lst *lst_c(lst *l);

lst_stat lst_g_i(lst *l, un v, size_t *idx);

typedef ssize_t lst_cmp_fn(un a, un b);

void lst_s(lst *l, lst_cmp_fn fn);

typedef bool lst_eq_fn(un a, un b);

bool lst_eq(const lst *restrict a, const lst *restrict b, lst_eq_fn fn);

lst_stat lst_ab(lst *l, un d);

lst_stat lst_af(lst *l, un d);

lst_stat lst_sb(lst *l, un *d);

lst_stat lst_sf(lst *l, un *d);

void lst_li_d(lst *l, te *li);

void lst_f(lst *l);
