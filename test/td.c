
#include "../src/td.h"

int main(void) {
    tds *s = tds_i();
    tdr *r = tds_g(s, false);
    tds_a(s, r);
    /*
    r = tds_g(s, true);
    tdr *rr = tds_g(s, false);
    tds_a(s, r);
    tds_a(s, rr);
    */
    tds_f(s);
    return 0;
}
