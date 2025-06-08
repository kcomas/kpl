
#pragma once

#include <stdlib.h>
#include <string.h>

typedef struct _tbl_itm {
    struct _tbl_itm *next;
    void *data;
    char str[]; // null term
} tbl_itm;

inline tbl_itm *tbl_itm_i(const char *const str, void *data) {
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

#define BRS 0.85 // bucks resize %

typedef struct {
    size_t is, il, bs, bu; // itms size, itms len, bucks size, bucks used
    tbl_itm **bucks, **itms;
} tbl;
