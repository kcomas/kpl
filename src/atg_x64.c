
#include "atg_x64.h"

bool atg_x64_enq(const te *an) {
    switch (an->d[2].u4) {
        case AST_CLS(O):
            return an->d[4].u4 == OC(CST) && an->d[6].p && ((te*) an->d[6].p)->d[2].u4 == AST_CLS(L);
        case AST_CLS(L):
            return true;
        default:
            break;
    }
    return false;
}
