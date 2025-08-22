
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
   te *ct = ns_type_fp(ns_type_h(TYPE(UN), 2, "v", ns_type_s(TYPE(SG)), "e", ns_type_v(TYPE(ER), ns_type_s(TYPE(SG)))), 1, "s", ns_type_s(TYPE(SG)));
   A(type_eq(nte->d[1].p, ct), "inv type");
   te_f(ct);
   for (size_t i = 0; i < n; i++) mc_f(m[i]);
}

T(s_ls) {
    mc *s = mc_i_cstr(".", &al_mc);
    te *u = ns_s_ls(&s);
    A(u->d[0].u6, "inv un key");
    vr *v = u->d[1].p;
    A(v->l > 1, "inv dir len");
    for (size_t i = 0; i < v->l; i++) printf("%s ", ((mc*) v->d[i].p)->d);
    putchar('\n');
    mc_f(s);
    te_f(u);
    s = mc_i_cstr("fileshouldnotexist", &al_mc);
    u = ns_s_ls(&s);
    A(!u->d[0].u6, "inv un key");
    err_p(u->d[1].p, 0, true);
    mc_f(s);
    te_f(u);
}
