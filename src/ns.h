
#pragma once

#include "./lib/tbl.h"
#include "./lib/type.h"
#include "./lib/err.h"

te *ns_type_s(type t);

// char*, type
te *ns_type_h(type t, size_t n, ...);

// char*, type
te *ns_type_fp(te *rt, size_t n, ...);

te *ns_un(size_t k, un v, frfn des);

// ns entry te[mc;type;fn;tbl]

// char*
void ns_a(te *type, void *any, size_t n, ...);

#ifndef NS_MAX_L
    #define NS_MAX_L 5
#endif

// returns ns entry null for error
te *ns_n(size_t n, mc *m[NS_MAX_L]);

void ns_p(void);

// declarations needed for testing
te *ns_f_rs(mc *s);
