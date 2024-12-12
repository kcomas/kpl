
#pragma once

#include "def.h"
#include "lst.h"

#define TBL_STAT(N) TBL_STAT_##N

typedef enum {
    TBL_STAT(OK)
} tbl_stat;

typedef size_t hhfn(un u); // hash

typedef bool cmpfn(un a, un b); // compare

// hashmap
typedef struct {
    ssize_t r;
    size_t u; // used
    alfn *at;
    frfn *ft;
    hhfn *hf;
    cmpfn *cf;
    lst *i; // items
    te *b; // buckets
} tbl;

tbl *tbl_i(alfn *at, frfn *ft, hhfn *hf, cmpfn *cf, lst *i, te *b);

tbl_stat tbl_g_i(tbl *t, un k, un *v);

tbl_stat tbl_a(tbl **t, un k, un v);

tbl_stat tbl_s(tbl *t, un k, un *v);

void tbl_f(tbl *t);
