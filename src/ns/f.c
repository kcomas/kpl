
#include "../ns.h"

static te *rs(mc *s) {
    (void) s;
    return NULL;
}

void ns_f_b() {
    ns_a(ns_type_fp(ns_type_h(TYPE(UN), 2, "v", ns_type_s(TYPE(SG)), "e", ns_type_s(TYPE(ER))), 1, "s", ns_type_s(TYPE(SG))), rs, 2, "f", "rs");
}
