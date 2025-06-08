
#include "tkn_t.h"
#include "../../src/lib/psr.h"

#define PCUST(N) PCUST_##N

typedef enum {
    PCUST(INT) = PARSER(USR),
    PCUST(FLT),
    PCUST(SUM),
    PCUST(SUB),
} pust;

#define NODE_TYPE(N) NODE_TYPE_##N

typedef enum {
    NODE_TYPE(ROOT),
    NODE_TYPE(INT),
    NODE_TYPE(FLT),
    NODE_TYPE(OP),
    NODE_TYPE(LST)
} node_type;

void psr_p(tbl *t, size_t idnt);

void node_f(void *p);

tbl *psr_mktbl(void);

void psr_entry_f(void *p);

// node te[par;type;tkn;....]
psr_stat psr_int_i(psr *const p, te **n);

// node te[par;type;tkn;tkn;tkn;....]
psr_stat psr_flt_i(psr *const p, te **n);

psr_stat psr_val_m(psr *const p, te *const nh, te *const n);

// node te[par;type;left;right]
psr_stat psr_op_i(psr *const p, te **n);

psr_stat psr_op_m(psr *const p, te *const nh, te *const n);

// node te[par;type;tkn_s;lst[node]]
psr_stat psr_lst_i(psr *const p, te **n);

psr_stat psr_lst_e(psr *const p, te *const e, te *const n);

void node_p(const te *const n, size_t idnt);

void node_f(void *p);
