
#include "../ns.h"

static int ac = 0;

static char **av = NULL;

void ns_args(int argc, char *argv[]) {
    ac = argc;
    av = argv;
}

vr *ns_s_a(void) {
    vr *a = vr_i(ac, &al_vr, (void*) mc_f);
    for (ssize_t i = 0; i < ac; i++) vr_ab(&a, P(mc_i_cstr(av[i], &al_mc)));
    return a;
}

void ns_s_b() {
    ns_a(ns_type_fp(ns_type_v(TYPE(VR), ns_type_s(TYPE(SG))), 0), ns_s_a, 2, "s", "a");
}
