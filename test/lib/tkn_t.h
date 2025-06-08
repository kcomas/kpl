
#include "../../src/lib/tkn.h"
#include <ctype.h>
#include <stdio.h>

#define TCUST(N) TCUST_##N

typedef enum {
    TCUST(NL) = TOKEN(USR),
    TCUST(WS),
    TCUST(NUM),
    TCUST(SEMI),
    TCUST(VAR)
} tcust;

void tkn_m_p(const te *const m, const mc *const s);

size_t tkn_sh(un v);

tbl *tkn_mktbl(void);

void tkn_entry_free(void *v);

tkn_stat tkn_df(tkn *const t, te *const m);

tkn_stat tkn_num(tkn *const t, te *const m);

tkn_stat tkn_nl(tkn *const t, te *const m);

tkn_stat tkn_ft(tkn *const t, te *const m);

tkn_stat tkn_ws(tkn *const t, te *const m);

void tkn_standard(tkn *const t);
