
#include "tbl.h"

static size_t sdbm(const char *str) {
    size_t h = 0;
    char c;
    while ((c = *str++)) h = c + (h << 6) + (h << 16) - h;
    return h;
}

extern inline tbl_itm *tbl_itm_i(const char *const str, void *const data);

extern inline void tbl_itm_f(tbl_itm *ti, const tbl_itm_data_f *const fn);

extern inline void tbl_f(tbl *t, const tbl_itm_data_f *const fn);

#ifndef BUCKS_RSIZE
    #define BUCKS_RSIZE 0.85 // bucks resize %
#endif

#ifndef BUCKS_RSIZE_MUL
    #define BUCKS_RSIZE_MUL 2 // mul size by
#endif

tbl_stat tbl_op(tbl **tl, const char *const str, void *const data, tbl_itm **ti, const tbl_itm_data_f *const fn, uint8_t op_flgs) {
    // TODO resize
    const size_t h = sdbm(str);
    for (size_t i = 0; i < (*tl)->size; i++) {
        tbl_itm *cur = (*tl)->bucks[(h + i) % (*tl)->size];
        if (!cur) {
            if ((op_flgs & TBL_OP_FLG(FD)) || op_flgs & TBL_OP_FLG(RM)) return TBL_STAT(NF);
            (*tl)->bucks[(h + i) % (*tl)->size] = *ti = tbl_itm_i(str, data);
            if (!(*tl)->h) (*tl)->h = (*tl)->h->next = (*tl)->t = *ti;
            else (*tl)->t = (*tl)->t->next = *ti;
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
                tbl_itm_f(cur, fn);
            } else {
                fn(cur->data);
                cur->data = data;
                *ti = data;
            }
            break;
        }
    }
    return TBL_STAT(OK);
}
