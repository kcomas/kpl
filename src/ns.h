
#pragma once

#include "./lib/tbl.h"
#include "./lib/type.h"
#include "./lib/err.h"

te *ns_type_s(type t);

// char*, type
te *ns_type_h(type t, size_t n, ...);

// char*, type
te *ns_type_fp(te *rt, size_t n, ...);

// ns entry te[mc;type;fn;tbl]

// char*
void ns_a(te *type, void *any, size_t n, ...);

#ifndef NS_MAX_L
    #define NS_MAX_L 5
#endif

// returns ns entry
te *ns_n(size_t n, mc *m[NS_MAX_L]);

void ns_p(void);
