
#include "mod.h"

extern inline mod *mod_i(void);

mod_stat mod_lfile(mod *const m, const char *const path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) return MOD_STAT(FLF);
    if (fstat(fd, &m->src.sb) == -1) {
        close(fd);
        return MOD_STAT(FLF);
    }
    m->src.str = calloc(m->src.sb.st_size + 1, sizeof(char));
    if (read(fd, m->src.str, m->src.sb.st_size) != m->src.sb.st_size) {
        free(m->src.str);
        close(fd);
        return MOD_STAT(FLF);
    }
    m->src.path = calloc(strlen(path) + 1, sizeof(char));
    strcpy(m->src.path, path);
    close(fd);
    return MOD_STAT(OK);
}

#pragma GCC push_options
#pragma GCC optimize ("O0")

void mod_ag(mod *const m, uint8_t ng) {
    m->g = calloc(ng, sizeof(var));
    m->ng = ng;
}

#define MOD_SG_FN(CT, T) MOD_SG(CT, T) { m->g[i].T = T; }
#define MOD_LG_FN(CT, T) MOD_LG(CT, T) { return m->g[i].T; }

MOD_SG_FN(int64_t, i6)
MOD_LG_FN(int64_t, i6)

MOD_SG_FN(uint64_t, u6)
MOD_LG_FN(uint64_t, u6)

void mod_sg_var_sg(mod *const m, uint8_t i, var_sg *const sg) { var_sg_rci(sg); m->g[i].sg = sg; }
var_sg *mod_lg_var_sg(mod *const m, uint8_t i) { var_sg_rci(m->g[i].sg); return m->g[i].sg; }

void mod_sg_jf(mod *const m, uint8_t i, jit_fn *const jf) { m->g[i].jf = jf; }
jit_fn *mod_lg_jf(mod *const m, uint8_t i) { return m->g[i].jf; }

#pragma GCC pop_options

extern inline void mod_psrc(const mod *const m);

extern inline void mod_f(mod *m);
