
#include "../src/mod.h"
#include "../src/ast.h"

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    mod *m = mod_i();
    mod_lfile(m, argv[1]);
    m->fns = fn_node_i(NULL);
    ast_stat astat;
    ast_st as;
    ast_st_i(&as, m->src.str);
    if ((astat = ast_parse_stmts(&as, m->fns, m->fns->body, TFBLS)) != AST_STAT(OK)) {
        fn_node_p(m->fns, 0);
        putchar('\n');
        return astat;
    }
    fn_node_f(m->fns);
    mod_f(m);
    return 0;
}
