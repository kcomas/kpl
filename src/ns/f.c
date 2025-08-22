
#include "../ns.h"

te *ns_f_rs(mc **s) {
    int fd;
    if ((fd = open((char*) (*s)->d, O_RDONLY)) == -1) return ns_err(*s, "inv path", ns_un_er_sg_f);
    struct statx sx;
    if (statx(fd, "", AT_EMPTY_PATH, STATX_MODE | STATX_SIZE, &sx) == -1) {
        close(fd);
        return ns_err(*s, "inv path", ns_un_er_sg_f);
    }
    if (S_ISDIR(sx.stx_mode)) return ns_err(*s, "path is dir", ns_un_er_sg_f);
    mc *f = mc_i(sx.stx_size + sizeof(char), &al_mc);
    if (read(fd, f->d, sx.stx_size) == -1) {
        close(fd);
        mc_f(f);
        return ns_err(*s, "failed to read file", ns_un_er_sg_f);
    }
    close(fd);
    return ns_un(1, P(f), ns_un_er_sg_f);
}

void ns_f_b() {
    ns_a(ns_type_fp(ns_type_h(TYPE(UN), 2, "v", ns_type_s(TYPE(SG)), "e", ns_type_v(TYPE(ER), ns_type_s(TYPE(SG)))), 1, "s", ns_type_s(TYPE(SG))), ns_f_rs, 2, "f", "rs");
}
