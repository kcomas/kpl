
#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../ns.h"

static void un_er_sg_f(void *p) {
    te *u = p;
    if (!u->d[0].u6) err_f(u->d[1].p);
    else mc_f(u->d[1].p);
    u->af->f(u);
}

static void ns_f_rs_err_p(void *d) {
    mc *s = d;
    printf("\e[1m%s\e[0m", (char*) s->d);
}

static te *ns_f_rs_err(mc *s, const char *msg) {
    return ns_un(0, P(err_i(&al_err, ns_f_rs_err_p, (void*) mc_f, mc_c(s), msg)), un_er_sg_f);
}

te *ns_f_rs(mc *s) {
    int fd;
    if ((fd = open((char*) s->d, O_RDONLY)) == -1) return ns_f_rs_err(s, "inv path");
    struct statx sx;
    if (statx(fd, "", AT_EMPTY_PATH, STATX_MODE | STATX_SIZE, &sx) == -1) {
        close(fd);
        return ns_f_rs_err(s, "inv path");
    }
    if (S_ISDIR(sx.stx_mode)) return ns_f_rs_err(s, "path is dir");
    mc *f = mc_i(sx.stx_size + sizeof(char), &al_mc);
    if (read(fd, f->d, sx.stx_size) == -1) {
        close(fd);
        mc_f(f);
        return ns_f_rs_err(s, "failed to read file");
    }
    close(fd);
    return ns_un(1, P(f), un_er_sg_f);
}

void ns_f_b() {
    ns_a(ns_type_fp(ns_type_h(TYPE(UN), 2, "v", ns_type_s(TYPE(SG)), "e", ns_type_v(TYPE(ER), ns_type_s(TYPE(SG)))), 1, "s", ns_type_s(TYPE(SG))), ns_f_rs, 2, "f", "rs");
}
