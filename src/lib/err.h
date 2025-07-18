
#pragma once

#include "def.h"

extern const alfr al_err;

typedef void err_d_p(void *d);

typedef void err_d_f(void *d);

typedef struct {
    ssize_t r;
    const alfr *af;
    err_d_p *dp;
    err_d_f *df;
    void *d;
    const char *m;
} err;

err *err_i(const alfr *af, err_d_p dp, err_d_f df, void *d, const char *m);

void err_p(const err *e, bool nl);

void err_f(err *e);
