
#include "tkn.h"
#include "./lib/psr.h"

#define NODE_TYPE(N) NODE_TYPE_##N

typedef enum {
    NODE_TYPE(NONE),
    NODE_TYPE(ROOT),
    NODE_TYPE(VAR),
    NODE_TYPE(TYPE),
    NODE_TYPE(KEY),
    NODE_TYPE(INT),
    NODE_TYPE(FLT),
    NODE_TYPE(OP),
    NODE_TYPE(LST),
    NODE_TYPE(APLY),
    NODE_TYPE(SYM)
} node_id;

psr *psr_b(psr *p);

void psr_p(tbl *t, size_t idnt);

void psr_entry_f(void *p);

// node te[par;id;tkn]
psr_stat psr_var_i(psr *p, te **n);

// node te[par;id;tkn]
psr_stat psr_type_i(psr *p, te **n);

// node te[par;id;tkn]
psr_stat psr_key_i(psr *p, te **n);

// node te[par;id;tkn]
psr_stat psr_int_i(psr *p, te **n);

// node te[par;id;tkn;tkn;tkn]
psr_stat psr_flt_i(psr *p, te **n);

psr_stat psr_val_m(psr *p, te *nh, te *n);

// node te[par;id;tkn;left;right]
psr_stat psr_op_i(psr *p, te **n);

psr_stat psr_op_m(psr *p, te *nh, te *n);

// node te[par;id;tkn;lst[node]]
psr_stat psr_lst_i(psr *p, te **n);

psr_stat psr_lst_e(psr *p, te *e, te *n);

// node te[par;id;tkn;tgt;lst[node]]
psr_stat psr_aply_i(psr *p, te **n);

psr_stat psr_aply_m(psr *p, te *nh, te *n);

psr_stat psr_aply_e(psr *p, te *e, te *n);

// node te[par;id;tkn;tgt]
psr_stat psr_sym_i(psr *p, te **n);

void node_p(const te *n, size_t idnt);

void node_f(void *p);
