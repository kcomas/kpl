
#include "tkn_t.h"
#include "../../src/lib/psr.h"

#define PCUST(N) PCUST_##N

typedef enum {
    PCUST(INT) = PARSER(USR),
    PCUST(SUM),
    PCUST(WS)
} pust;

#define NODE_TYPE(N) NODE_TYPE_##N

typedef enum {
    NODE_TYPE(VAL),
    NODE_TYPE(OP)
} node_type;

void node_f(void *p);

tbl *psr_mktbl(void);

void psr_entry_free(void *p);

// node te[type;tkn;....]
psr_stat psr_val_i(psr *const p, te **n);

psr_stat psr_val_m(psr *const p, te **h, void ***c, te *const n);

// node te[....left;right]
psr_stat psr_op_i(psr *const p, te **n);

psr_stat psr_op_m(psr *const p, te **h, void ***c, te *const n);

void node_p(const te *const n, const mc *const s, size_t idnt);

void node_f(void *p);
