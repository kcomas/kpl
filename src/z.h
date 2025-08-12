
#pragma once

#define _GNU_SOURCE
#include <fcntl.h>
#include <sys/stat.h>
#include "ns.h"
#include "atg_x64.h"

#define Z_D_FLG(N) Z_D_FLG_##N

typedef enum {
    Z_D_FLG(P) = 1 << 0, // psr
    Z_D_FLG(A) = 1 << 1, // ast
    Z_D_FLG(F) = 1 << 2, // fld
    Z_D_FLG(C) = 1 << 3, // chk
    Z_D_FLG(O) = 1 << 4, // opt
    Z_D_FLG(G) = 1 << 5, // gen
    Z_D_FLG(S) = 1 << 6, // as
    Z_D_FLG(Y) = 1 << 7, // combined gen + as
    Z_D_FLG(E) = 1 << 8 // exec
} z_d_flgs;

void z_atg_rst(void); // testing

void z_atg_d_p(void);

err *z(mc *fn, tbl **et, uint16_t dflgs); // file name, exports tbl, dump flgs
