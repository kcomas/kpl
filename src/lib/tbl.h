
#pragma once

#include "mc.h"
#include "def.h"
#include "lst.h"

#define TBL_STAT(N) TBL_STAT_##N

typedef enum {
    TBL_STAT(OK),
    TBL_STAT(RES),
    TBL_STAT(OAE), // open address error
    TBL_STAT(NF) // not found
} tbl_stat;

typedef size_t hhfn(un k); // hash

typedef bool cmpfn(un a, un b); // compare

// hashmap open address
typedef struct {
    ssize_t r;
    const alfr *af;
    hhfn *hf;
    cmpfn *cf;
    lst *i; // items
    te *b; // buckets
} tbl;

tbl *tbl_i(const alfr *af, hhfn *hf, cmpfn *cf, lst *i, te *b);

tbl *tbl_i_t(const tbl *t);

tbl *tbl_c(tbl *t);

typedef bool tbl_cmp_fn(const te *restrict ea, const te *restrict eb);

bool tbl_eq(const tbl *restrict a, const tbl *restrict b, tbl_cmp_fn cmp);

size_t tbl_no_hsh(un d);

size_t tbl_mc_sdbm(un d);

bool tbl_un_eq(un a, un b);

bool tbl_mc_eq(un a, un b);

size_t tbl_g_l(const tbl *t);

size_t tbl_g_s(const tbl *t);

tbl_stat tbl_g_i(tbl *t, un k, te **kv);

tbl_stat tbl_a(tbl *t, te *kv);

tbl_stat tbl_s(tbl *t, un k, te **kv);

void tbl_f(tbl *t);
