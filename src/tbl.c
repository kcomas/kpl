
#include "tbl.h"

extern inline tbl_itm *tbl_itm_i(const char *const str, void *const data);

extern inline void tbl_itm_p(const tbl_itm *const ti, tbl_itm_data_p *fn);

extern inline void tbl_itm_f(tbl_itm *ti, tbl_itm_data_f *fn);

extern inline tbl* tbl_i(size_t size);

#ifndef BUCKS_RSIZE
    #define BUCKS_RSIZE 0.75 // bucks resize %
#endif

#ifndef BUCKS_RSIZE_MUL
    #define BUCKS_RSIZE_MUL 2 // mul size by
#endif

#define DJB2 size_t h = 5381; \
    while ((c = *str++)) h = ((h << 5) + h) + c;

#define SDBM size_t h = 0; \
    while ((c = *str++)) h = c + (h << 6) + (h << 16) - h;

#ifndef HFN
    #define HFN SDBM
#endif

static size_t hashfn(const char *str) {
    char c;
    HFN
    return h;
}

tbl_stat tbl_op(tbl **tl, const char *const str, void *const data, tbl_itm **ti, tbl_itm_data_f *fn, uint8_t op_flgs) {
    if ((double) (*tl)->len / (double) (*tl)->size > BUCKS_RSIZE) {
        tbl *nt = tbl_i((*tl)->size * BUCKS_RSIZE_MUL);
        tbl_itm *h = nt->h = (*tl)->h;
        nt->t = (*tl)->t;
        while (h) {
            const size_t hash = hashfn(h->str);
            size_t i;
            for (i = 0; i < nt->size; i++) {
                if (!nt->bucks[(hash + i) % nt->size]) {
                    nt->bucks[(hash + i) % nt->size] = h;
                    nt->len++;
                    break;
                }
            }
            if (i == nt->size) return TBL_STAT(OAE);
            h = h->next;
        }
        free(*tl);
        *tl = nt;
    }
    const size_t hash = hashfn(str);
    size_t i;
    for (i = 0; i < (*tl)->size; i++) {
        tbl_itm *cur = (*tl)->bucks[(hash + i) % (*tl)->size];
        if (!cur) {
            if ((op_flgs & TBL_OP_FLG(FD)) || op_flgs & TBL_OP_FLG(RM)) return TBL_STAT(NF);
            (*tl)->bucks[(hash + i) % (*tl)->size] = *ti = tbl_itm_i(str, data);
            if (!(*tl)->h) {
                (*tl)->h = *ti;
                (*tl)->h->next = (*tl)->t = *ti;
            } else (*tl)->t = (*tl)->t->next = *ti;
            (*tl)->len++;
            break;
        } else if (strcmp(cur->str, str) == 0) {
            if (op_flgs & TBL_OP_FLG(AD)) return TBL_STAT(AE);
            if (op_flgs & TBL_OP_FLG(RM)) {
                tbl_itm *h = (*tl)->h;
                if (cur == h) (*tl)->h = h->next;
                else {
                    while (cur != h->next) h = h->next;
                    h->next = cur->next;
                }
                (*tl)->bucks[(hash + i) % (*tl)->size] = NULL;
                tbl_itm_f(cur, fn);
                (*tl)->len--;
            } else {
                if (data) {
                    fn(cur->data);
                    cur->data = data;
                }
                *ti = cur;
            }
            break;
        }
    }
    if (i == (*tl)->size) return TBL_STAT(OAE);
    return TBL_STAT(OK);
}

extern inline void tbl_bucksp(const tbl *const tl, tbl_itm_data_p *fn);

extern inline void tbl_lstp(const tbl *const tl, tbl_itm_data_p *fn);

extern inline void tbl_f(tbl *t, tbl_itm_data_f *fn);
