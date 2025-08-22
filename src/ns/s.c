
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

#define MIN_DIRS 10

static ssize_t dir_s(un a, un b) {
    return strcmp((char*) ((mc*) a.p)->d, (char*) ((mc*) b.p)->d);
}

te* ns_s_ls(mc **d) {
    DIR *dp;
    struct dirent *ep;
    if (!(dp = opendir((char*) (*d)->d))) return ns_err(*d, "inv path", ns_un_er_vr_f);
    vr *v = vr_i(MIN_DIRS, &al_vr, (void*) mc_f);
    while ((ep = readdir(dp))) vr_ab(&v, P(mc_i_cstr(ep->d_name, &al_mc)));
    closedir(dp);
    vr_s(v, dir_s);
    return ns_un(1, P(v), ns_un_er_vr_f);
}

int64_t ns_s_r(mc **c) {
    return (int64_t) system((char*) (*c)->d);
}

void ns_s_b() {
    ns_a(ns_type_fp(ns_type_v(TYPE(VR), ns_type_s(TYPE(SG))), 0), ns_s_a, 2, "s", "a");
    ns_a(ns_type_fp(ns_type_h(TYPE(UN), 2, "v", ns_type_v(TYPE(VR), ns_type_s(TYPE(SG))), "e", ns_type_v(TYPE(ER), ns_type_s(TYPE(SG)))), 1, "d", ns_type_s(TYPE(SG))), ns_s_ls, 2, "s", "ls");
    ns_a(ns_type_fp(ns_type_s(TYPE(I6)), 1, "c", ns_type_s(TYPE(SG))), ns_s_r, 2, "s", "r");
}
