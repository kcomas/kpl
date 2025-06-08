
#pragma once

#include "kpl.h"

typedef struct _tbl_itm {
    struct _tbl_itm *prev, *next;
    void *data;
    char str[]; // null term
} tbl_itm;

inline tbl_itm *tbl_itm_i(al *const a, const char *const str, void *const data) {
    tbl_itm *ti = ala(a, sizeof(tbl_itm) + sizeof(void*) + strlen(str) + 1);
    strcpy(ti->str, str);
    ti->data = data;
    return ti;
}

typedef void tbl_itm_data_p(void *data);

inline void tbl_itm_p(const tbl_itm *const ti, tbl_itm_data_p *fn) {
    printf("%s", ti->str);
    if (fn) fn(ti->data);
}

typedef void tbl_itm_data_f(void *data);

inline void tbl_itm_f(tbl_itm *ti, tbl_itm_data_f *fn) {
    FNNF(ti->data, fn);
    alf(ti);
}

#ifndef TBL_I_SIZE
    #define TBL_I_SIZE 10
#endif

typedef struct {
    size_t size, len;
    tbl_itm *h, *t, *bucks[];
} tbl; // open address

inline tbl* tbl_i(al *const a, size_t size) {
    tbl* tl = ala(a, sizeof(tbl) + sizeof(tbl_itm*) * size);
    tl->size = size;
    return tl;
}

#define TBL_STAT(N) TBL_STAT_##N

typedef enum {
   TBL_STAT(OK),
   TBL_STAT(NF), // not found
   TBL_STAT(AE), // already exists
   TBL_STAT(OAE), // open address error
   TBL_STAT(NOFN) // no function for removing data
} tbl_stat;

#define TBL_OP_FLG(N) TBL_OP_FLG_##N

typedef enum {
    TBL_OP_FLG(UT) = (1 << 0), // upsert
    TBL_OP_FLG(FD) = (1 << 1), // find
    TBL_OP_FLG(AD) = (1 << 2), // add only
    TBL_OP_FLG(RM) = (1 << 3) // remove
} tbl_op_flg;

tbl_stat tbl_op(al *const a, tbl **tl, const char *const str, void *const data, tbl_itm **ti, tbl_itm_data_f *fn, uint8_t op_flgs);

inline void tbl_bucksp(const tbl *const tl, tbl_itm_data_p *fn, char sep) {
    for (size_t i = 0; i < tl->size; i++) {
        printf("%lu:", i);
        if (tl->bucks[i]) tbl_itm_p(tl->bucks[i], fn);
        if (i < tl->size - 1) putchar(sep);
    }
}

inline void tbl_lstp(const tbl *const tl, tbl_itm_data_p *fn, char sep) {
    tbl_itm *h = tl->h;
    while (h) {
        tbl_itm_p(h, fn);
        if (h->next) putchar(sep);
        h = h->next;
    }
}

// print reverse
inline void tbl_lstpr(const tbl *const tl, tbl_itm_data_p *fn, char sep) {
    tbl_itm *t = tl->t;
    while (t) {
        tbl_itm_p(t, fn);
        if (t->prev) putchar(sep);
        t = t->prev;
    }
}

inline void tbl_f(tbl *tl, tbl_itm_data_f *fn) {
    LST_F(tl, tbl_itm, tbl_itm_f, fn);
    alf(tl);
}
