
#include "tbl.h"

extern inline tbl_itm *tbl_itm_i(al *const a, const char *const str, void *const data);

extern inline void tbl_itm_p(const tbl_itm *const ti, tbl_itm_data_p *fn);

extern inline void tbl_itm_f(tbl_itm *ti, tbl_itm_data_f *fn);

extern inline tbl* tbl_i(al *const a, size_t size);

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

tbl_stat tbl_op(al *const a, tbl **tl, const char *const str, void *const data, tbl_itm **ti, tbl_itm_data_f *fn, uint8_t op_flgs) {
    if ((double) (*tl)->len / (double) (*tl)->size > BUCKS_RSIZE) {
        tbl *nt = tbl_i(a, (*tl)->size * BUCKS_RSIZE_MUL);
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
        alf(*tl);
        *tl = nt;
    }
    const size_t hash = hashfn(str);
    size_t i;
    for (i = 0; i < (*tl)->size; i++) {
        tbl_itm *cur = (*tl)->bucks[(hash + i) % (*tl)->size];
        if (!cur) {
            if ((op_flgs & TBL_OP_FLG(FD)) || op_flgs & TBL_OP_FLG(RM)) return TBL_STAT(NF);
            (*tl)->bucks[(hash + i) % (*tl)->size] = *ti = tbl_itm_i(a, str, data);
            LST_A((*tl), *ti);
            break;
        } else if (strcmp(cur->str, str) == 0) {
            if (op_flgs & TBL_OP_FLG(AD)) return TBL_STAT(AE);
            if (op_flgs & TBL_OP_FLG(RM)) {
                if (!fn) return TBL_STAT(NOFN);
                LST_R((*tl), tbl_itm, cur, tbl_itm_f, fn);
                (*tl)->bucks[(hash + i) % (*tl)->size] = NULL;
            } else {
                if (data) {
                    if (!fn) return TBL_STAT(NOFN);
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

extern inline void tbl_bucksp(const tbl *const tl, tbl_itm_data_p *fn, char sep);

extern inline void tbl_lstp(const tbl *const tl, tbl_itm_data_p *fn, char sep);

extern inline void tbl_lstpr(const tbl *const tl, tbl_itm_data_p *fn, char sep);

extern inline void tbl_f(tbl *t, tbl_itm_data_f *fn);
