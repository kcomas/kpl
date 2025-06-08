
#include "tbl.h"

static size_t sdbm(const char *str) {
    size_t h = 0;
    char c;
    while ((c = *str++)) h = c + (h << 6) + (h << 16) - h;
    return h;
}

extern inline tbl_itm *tbl_itm_i(const char *const str, void *const data);

extern inline void tbl_itm_f(tbl_itm *ti, tbl_itm_data_f *fn);

extern inline void tbl_f(tbl *t, tbl_itm_data_f *fn);

#ifndef BUCKS_RSIZE
    #define BUCKS_RSIZE 0.85 // bucks resize %
#endif

#ifndef BUCKS_RSIZE_MUL
    #define BUCKS_RSIZE_MUL 2 // mul size by
#endif

#ifndef OPN_ADDR_SRCH
    #define OPN_ADDR_SRCH 10 // bucks size / OPN_ADDR_SRCH searching
#endif

static void add_tbl(tbl *tl, tbl_itm **ins, tbl_itm *ti) {
    *ins = ti;
    if (!tl->h) tl->h = tl->h->next = tl->t = ti;
    else tl->t = tl->t->next = ti;
    tl->len++;
}

tbl_stat tbl_op(tbl **tl, const char *const str, void *const data, tbl_itm **ti, uint8_t op_flgs) {
    // TODO resize
    size_t h = sdbm(str) % (*tl)->size;
    if ((*tl)->bucks[h] == NULL) {
        if ((op_flgs & TBL_OP_FLG(FD)) || (op_flgs & TBL_OP_FLG(RM))) return TBL_STAT(NF);
        *ti = tbl_itm_i(str, data);
        add_tbl(*tl, &(*tl)->bucks[h], *ti);
        return TBL_STAT(OK);
    }
    tbl_itm **b = &(*tl)->bucks[h];
    while (*b) {
        if (strcmp((*b)->str, str) == 0) {
            if (op_flgs & TBL_OP_FLG(FD)) {
                *ti = *b;
                return TBL_STAT(OK);
            } else if (op_flgs & TBL_OP_FLG(AD)) return TBL_STAT(AE);
            else if (op_flgs & TBL_OP_FLG(RM)) {
                // TODO remove
                return TBL_STAT(OK);
            }
            (*b)->data = data;
            return TBL_STAT(OK);
        }
        *b = (*b)->next;
    }
    if ((op_flgs & TBL_OP_FLG(FD)) || (op_flgs & TBL_OP_FLG(RM))) return TBL_STAT(NF);
    // TODO add
}
