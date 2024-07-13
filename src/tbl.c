
#include "tbl.h"

static size_t sdbm(const char *str) {
    size_t h = 0;
    char c;
    while ((c = *str++)) h = c + (h << 6) + (h << 16) - h;
    return h;
}

extern inline tbl_itm *tbl_itm_i(const char *const str, void *data);

extern inline void tbl_itm_f(tbl_itm *ti, tbl_itm_data_f *fn);
