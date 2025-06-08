
#include "mod.h"

mod_stat mod_file(mod *const m, const char *const path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) return MOD_STAT(FLF);
    if (statx(fd, "", AT_EMPTY_PATH, STATX_BASIC_STATS | STATX_BTIME, &mod.src.info) == -1) {
        close(fd);
        return MOD_STAT(FLF);
    }
    mod.src.data = calloc(mod.src.info.stx_size + 1, sizeof(char));
    if (read(fd, mod.src.str, mod.src.info.stx_size) != mod.src.info.stx_size) {
        free(mod.src.str);
        close(fd);
        return MOD_STAT(FLF);
    }
    mod.src.path = calloc(strlen(path) + 1, sizeof(char);
    strcpy(mod.src.path, path);
    close(fd);
    return MOD_STAT(OK);
}
