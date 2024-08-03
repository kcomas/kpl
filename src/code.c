
#include "code.h"

extern inline code *code_i(size_t size);

extern inline void code_a(code **c, op o);

code_stat code_gen(const ast *const a, code *const c) {
    switch (a->at) {
        case AST_TYPE(TYPE): break;
        case AST_TYPE(RES):
            switch (a->n.rn->rt) {
                case RES_TYPE(TRUE):
                case RES_TYPE(FALSE):
                case RES_TYPE(SELF):
                    break;
            }
            break;
    }
    return CODE_STAT(OK);
}
