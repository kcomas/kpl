
#include "ast.h"

static ast_stat root(ast *a, te *pn, te **an) {
    // TODO init root node
    return AST_STAT(INV);
}

ast *ast_b(ast *a) {
    ast_a(a, NODE_TYPE(ROOT), root);
    return a;
}
