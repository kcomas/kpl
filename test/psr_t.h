
#pragma once

#include "../src/psr.h"
#include "t.h"

psr *bpsr(const char *pgm);

te *psr_r(psr *p);

#define TPGM(N) TPGM_##N

extern const char *TPGM(aplyopadd);

extern const char *TPGM(typetype);

extern const char *TPGM(fnadd3);

extern const char *TPGM(fnf6muli6cstdiv);

extern const char *TPGM(apltypefn);

extern const char *TPGM(facloop);

extern const char *TPGM(facrec);

extern const char *TPGM(fibrec);

extern const char *TPGM(ackrec);

extern const char *TPGM(scope);

extern const char *TPGM(efn);

extern const char *TPGM(teadd);

extern const char *TPGM(vrmul);

extern const char *TPGM(st);

extern const char *TPGM(import);

extern const char *TPGM(lfac);

extern const char *TPGM(istr);

extern const char *TPGM(fnscope);

extern const char *TPGM(or);

extern const char *TPGM(un);

extern const char *TPGM(unmatch);

extern const char *TPGM(ns);

extern const char *TPGM(iline);

extern const char *TPGM(unsg);

extern const char *TPGM(vridx);

extern const char *TPGM(nscope);

extern const char *TPGM(cj);

extern const char *TPGM(leap);

//extern const char *TPGM(rtcmd);
