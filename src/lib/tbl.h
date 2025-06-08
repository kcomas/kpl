
#pragma once

#include "def.h"
#include "lst.h"

#define TBL_STAT(N) TBL_STAT_##N

typedef enum {
    TBL_STAT(OK),
    TBL_STAT(RES),
    TBL_STAT(UP),
    TBL_STAT(OAE), // open address error
    TBL_STAT(NF) // not found
} tbl_stat;

typedef size_t hhfn(un k); // hash

typedef bool cmpfn(un a, un b); // compare

// hashmap open address
typedef struct {
    ssize_t r;
    alfn *ta;
    frfn *tf;
    hhfn *hf;
    cmpfn *cf;
    lst *i; // items
    te *b; // buckets
} tbl;

tbl *tbl_i(alfn *at, frfn *ft, hhfn *hf, cmpfn *cf, lst *i, te *b);

tbl *tbl_c(tbl *t);

size_t tbl_g_l(const tbl *const t);

size_t tbl_g_s(const tbl *const t);

tbl_stat tbl_g_i(tbl *t, un k, te **kv);

tbl_stat tbl_a(tbl *t, te *kv);

tbl_stat tbl_s(tbl *t, un k, te **kv);

void tbl_f(tbl *t);
