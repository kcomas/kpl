
#include "../../src/lib/tkn.h"
#include <ctype.h>
#include <stdio.h>

size_t tkn_sh(un v);

tbl *tkn_mktbl(void);

void tkn_entry_free(void *v);

tkn_stat tkn_df(tkn *const t, te *const m);

tkn_stat tkn_num(tkn *const t, te *const m);

tkn_stat tkn_nl(tkn *const t, te *const m);

tkn_stat tkn_ft(tkn *const t, te *const m);

tkn_stat tkn_ws(tkn *const t, te *const m);
