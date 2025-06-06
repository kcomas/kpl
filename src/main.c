
#include "z.h"

static int usage(const char *name) {
    printf("\e[1musage: %s [opts] file.kpl\n", name);
    printf(" -d[step] dumps output from: p(psr) a(ast) f(fld) c(chk) o(opt) g(gen) s(as)\n");
    printf(" -h show this msg\n\e[0m");
    return 1;
}

static const uint8_t dopts[26] = {
    ['p' - 'a'] = Z_D_FLG(P),
    ['a' - 'a'] = Z_D_FLG(A),
    ['f' - 'a'] = Z_D_FLG(F),
    ['c' - 'a'] = Z_D_FLG(C),
    ['o' - 'a'] = Z_D_FLG(O),
    ['g' - 'a'] = Z_D_FLG(G),
    ['s' - 'a'] = Z_D_FLG(S)
};

int main(int argc, char *argv[]) {
    ssize_t i = 1, x = 1, o = 0;
    uint8_t dflgs = 0;
    while (i < argc && argv[i][0] == '-') {
        switch (argv[i][x]) {
            case 'h':
                return usage(argv[0]);
            case 'd':
                while (argv[i][++x] != '\0') {
                    o = argv[i][x] - 'a';
                    if (o < 0 || o > 25 || !dopts[o]) {
                        printf("\e[1;91minv -d opt: %c\n\e[0m", argv[i][x]);
                        return usage(argv[0]);
                    }
                    dflgs |= dopts[o];
                }
                if (!dflgs) {
                    printf("\e[1;91minv -d\n\e[0m");
                    return usage(argv[0]);
                }
                x = 1;
                break;
            case '-':
                i++;
                goto run;
            default:
                printf("\e[1;91minv opt -%c\n\e[0m", argv[i][x]);
                return usage(argv[0]);
        }
        i++;
    }
    run:
    if (i == argc) {
        HERE("TODO REPL");
        return usage(argv[0]);
    }
    mc *fn = mc_i_cstr(argv[i], &al_mc);
    tbl *et = NULL;
    err *e = z(fn, &et, dflgs);
    mc_f(fn);
    opt_exp_tbl_f(et);
    if (e) {
        err_p(e);
        err_f(e);
        return 1;
    }
    return 0;
}
