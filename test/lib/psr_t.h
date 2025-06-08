
#include "tkn_t.h"
#include "../../src/lib/psr.h"

#define PCUST(N) PCUST_##N

typedef enum {
    PCUST(INT) = PARSER(USR)
} pust;

psr_stat psr_df(psr *const p, te **const n);

void psr_ef(void *p);

tbl *psr_mktbl(void);

psr_stat psr_ign(psr *const p, te **const n);

psr_stat psr_val(psr *const p, te **const n);

psr_stat psr_df(psr *const p, te **const n);
