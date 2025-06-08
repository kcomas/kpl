
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct _tbl_itm {
    struct _tbl_itm *next;
    void *data;
    char str[]; // null term
} tbl_itm;

inline tbl_itm *tbl_itm_i(const char *const str, void *const data) {
    tbl_itm *ti = calloc(1, sizeof(tbl_itm) + sizeof(void*) + strlen(str) + 1);
    strcpy(ti->str, str);
    ti->data = data;
    return ti;
}

typedef void tbl_itm_data_f(void *data);

inline void tbl_itm_f(tbl_itm *ti, tbl_itm_data_f *fn) {
    fn(ti->data);
    free(ti);
}

#ifndef TBL_START_SIZE
    #define TBL_START_SIZE 10
#endif

typedef struct {
    size_t size, len;
    tbl_itm *h, *t, *bucks[];
} tbl; // open address

inline tbl* tbl_i(size_t size) {
    tbl* tl = calloc(1, sizeof(tbl) + sizeof(tbl_itm*) * size);
    tl->size = size;
    return tl;
}

inline void tbl_f(tbl *tl, tbl_itm_data_f *fn) {
    tbl_itm *h = tl->h;
    while (h) {
        tbl_itm *tmp = h;
        h = h->next;
        tbl_itm_f(tmp, fn);
    }
    free(tl);
}

#define TBL_STAT(N) TBL_STAT_##N

typedef enum {
   TBL_STAT(OK),
   TBL_STAT(NF), // not found
   TBL_STAT(AE), // already exists
   TBL_STAT(OAE) // open address error
} tbl_stat;

#define TBL_OP_FLG(N) TBL_OP_FLG_##N

typedef enum {
    TBL_OP_FLG(FD) = (1 << 0), // find
    TBL_OP_FLG(AD) = (1 << 1), // add only
    TBL_OP_FLG(RM) = (1 << 2) // remove
} tbl_op_flg;

tbl_stat tbl_op(tbl **tl, const char *const str, void *const data, tbl_itm **ti, tbl_itm_data_f *fn, uint8_t op_flgs);
