
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

extern inline void mod_psrc(const mod *const m);

extern inline void mod_f(mod *m);
