
#pragma once

// link list macros

typedef struct _lst_itm_ex {
    struct _lst_itm_ex *prev, *next;
    void *data;
    // your data
} lst_itm_ex;

typedef struct _lst_ex {
    size_t len;
    lst_itm_ex *h, *t;
    // your data
} lst_ex;

#define LST_A(L, I) if (!L->h) L->t = L->h = I; \
    else { \
        (I)->prev = L->t; \
        L->t = L->t->next = I; \
    } \
    L->len++

#define LST_R(L, ITT, RTGT, ITMF, DATAF) ITT *h = L->h; \
    if (RTGT == h) { \
        L->h = h->next; \
        L->h->prev = NULL; \
    } else { \
        while (RTGT != h->next) h = h->next; \
        if (RTGT->next) RTGT->next->prev = h; \
        h->next = RTGT->next; \
    } \
    ITMF(RTGT, DATAF); \
    L->len--

#define LST_P(L, ITT, ITMP, STATE, DATAP, IDNT, SEP) ITT *h = L->h; \
    while (h) { \
        putchar(SEP); \
        PCX(' ', idnt); \
        ITMP(STATE, h, DATAP, IDNT); \
        h = h->next; \
    }

#define LST_F(L, ITT, ITMF, DATAF) ITT *h = L->h; \
    while (h) { \
        ITT *tmp = h; \
        h = h->next; \
        ITMF(tmp, DATAF); \
    } \
    free(L)
