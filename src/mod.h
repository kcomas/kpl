
#pragma once

#include "kpl.h"
#include "var.h"

#define MOD_STAT(N) MOD_STAT_##N

typedef enum {
    MOD_STAT(OK),
    MOD_STAT(FLF) // file load fail
} mod_stat;

typedef struct {
    uint8_t ng; // number of globals
    struct {
        struct stat sb;
        char *path, *str;
    } src;
    fn_node *fns; // ast root
    code *c;
    jit *j;
    var *g; // globals
} mod;

inline mod *mod_i(void) {
    return calloc(1, sizeof(mod));
}

// load file
mod_stat mod_lfile(mod *const m, const char *const path);

// allocate globals
void mod_ag(mod *const m, uint8_t ng);

void mod_sg_i6(mod *const m, uint8_t i, int64_t i6);

int64_t mod_lg_i6(mod *const m, size_t i);

// print src
inline void mod_psrc(const mod *const m) {
    if (m->src.path != NULL) printf("%s\n", m->src.path);
    printf("%s", m->src.str);
}

inline void mod_f(mod *m) {
    FNN(m->src.path);
    FNN(m->src.str);
    // TODO use fn scope symtbl to free globals
    free(m->g);
    free(m);
}
