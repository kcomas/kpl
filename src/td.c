
#include "td.h"

extern inline void tdr_stk_i(tdr *const r);

extern inline tdr *tdr_i(tds *const s);

extern inline void tdr_f(tdr *r, void *fn);

extern inline tds *tds_i(void);

extern inline void tds_a(tds *volatile s, tdr *const r);

extern inline tdr *tds_g(tds *volatile s, bool stk);

extern inline void tds_f(tds *s);
