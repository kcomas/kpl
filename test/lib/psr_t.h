
#include "tkn_t.h"
#include "../../src/lib/psr.h"

#define PCUST(N) PCUST_##N

typedef enum {
    PCUST(INT) = PARSER(USR)
} pust;

tbl *psr_mktbl(void);

void psr_entry_free(void *p);
