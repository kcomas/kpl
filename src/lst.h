
#pragma once

// link list macros

typedef struct _lst_itm_ex {
    struct _lst_itm_ex *next;
    void *data;
    // your data
} lst_itm_ex;

typedef struct _lst_ex {
    size_t len;
    lst_itm_ex *h, *t;
    // your data
} lst_ex;

#define LST_A(L, I) if (!L->h) { \
    L->t = L->h = I; \
    } else L->t = L->t->next = I; \
    L->len++

#define LST_R(L, ITT, RTGT, ITMF, DATAF) ITT *h = L->h; \
    if (RTGT == h) L->h = h->next; \
    else { \
        while (RTGT != h->next) h = h->next; \
        h->next = RTGT->next; \
    } \
    tbl_itm_f(RTGT, DATAF); \
    L->len--


#define LST_F(L, ITT, ITMF, DATAF) ITT *h = L->h; \
    while (h) { \
        ITT *tmp = h; \
        h = h->next; \
        ITMF(tmp, DATAF); \
    } \
    free(L)
