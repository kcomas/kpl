
#include "../src/ns.h"
#include "t.h"

T(print) {
    ns_p();
}

T(f_rs) {
   mc *m[NS_MAX_L] = { mc_i_cstr("f", &al_mc), mc_i_cstr("rs", &al_mc) };
   size_t n = 2;
   te *nte = ns_n(2, m);
   A(nte, "inv ns tbl entry");
   te *ct = ns_type_fp(ns_type_h(TYPE(UN), 2, "v", ns_type_s(TYPE(SG)), "e", ns_type_s(TYPE(ER))), 1, "s", ns_type_s(TYPE(SG)));
   A(type_eq(nte->d[1].p, ct), "inv type");
   te_f(ct);
   for (size_t i = 0; i < n; i++) mc_f(m[i]);
}
