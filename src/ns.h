
#pragma once

#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "./lib/mc.h"
#include "./lib/vr.h"
#include "./lib/tbl.h"
#include "./lib/type.h"
#include "./lib/err.h"

te *ns_type_s(type t);

te *ns_type_v(type t, te *vt);

// char*, type
te *ns_type_h(type t, size_t n, ...);

// char*, type
te *ns_type_fp(te *rt, size_t n, ...);

te *ns_un(size_t k, un v, frfn des);

// ns entry te[mc;type;fn;tbl]

// char*
void ns_a(te *type, void *any, size_t n, ...);

#ifndef NS_MAX_L
    #define NS_MAX_L 5
#endif

// returns ns entry null for error
te *ns_n(size_t n, mc *m[NS_MAX_L]);

void ns_p(void);

void ns_un_er_sg_f(void *p);

void ns_un_er_vr_f(void *p);

te *ns_err(mc *s, const char *msg, frfn des);

void ns_args(int argc, char *argv[]);

// declarations needed for testing
te *ns_f_rs(mc **s);

vr *ns_s_a(void);

te* ns_s_ls(mc **d);

int64_t ns_s_r(mc **c);
