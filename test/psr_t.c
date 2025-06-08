
#include "psr_t.h"

static const alfr pm = { .a = malloc, .f = free };

static tbl *tkn_mktbl(void) {
    lst *tl = lst_i(&al_lst, &al_te, (void*) te_f);
    te *b = te_i(10, &al_te, NULL);
    tbl *t = tbl_i(&al_tbl, tkn_sh, c4_eq, tl, b);
    return t;
}

static tbl *mktbl(void) {
    lst *tl = lst_i(&al_lst, &al_te, (void*) te_f);
    te *b = te_i(10, &al_te, NULL);
    tbl *t = tbl_i(&al_tbl, tbl_no_hsh, tbl_un_eq, tl, b);
    return t;
}

static psr *bp = NULL;

static __attribute__((constructor)) void psr_con(void) {
    tkn *t = tkn_b(tkn_i(&pm, &al_te, &pm, tkn_mktbl, tkn_df, mc_i(0, &al_mc)));
    vr *v = vr_i(10, &al_vr, (void*) te_f);
    bp = psr_b(psr_i(&pm, &al_te, &al_lst, &pm, psr_entry_f, mktbl, t, v));
    psr_a(bp, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(bp->tt, TOKEN(UN), "Σ", tkn_ft));
}

static __attribute__((destructor)) void psr_des(void) {
    psr_f(bp);
}

psr *bpsr(const char *pgm) {
    return psr_i_psr(bp, mc_i_cstr(pgm, &al_mc));
}

const char *TPGM(aplyopadd) = "+(1;2)";

const char *TPGM(typetype) = "FN(I6`x;F6`y;FN(U6`z;I6))";

const char *TPGM(fnadd3) = "{f::FN(I6`a;I6`b;I6`c;I6)${-a+b+c};f(1;2;3)/p1}()";

const char *TPGM(fnf6muli6cstdiv) = "{f::FN(F6`x;F6`y;U6`z;F6)${/(x*y;F6$z)};f(4.4;6.6;U6$2)/p1}()";

const char *TPGM(apltypefn) = "{f::FN${I6(a)+I6(b)*a};f(3;4)/p1}()";

const char *TPGM(facloop) = "{\n\tv::5\n\tf/e::1\n\t>(v;1)@{\n\t\tf*:v\n\t\tv-:1\n\t}\n\tf/p1\n}()";

const char *TPGM(facrec) = "{\n"
                     " fac::FN(U6`n;U6)${\n"
                     "  {n>1}?{\n"
                     "   n*fac(n-1)\n"
                     "   U6$1\n"
                     "  }\n"
                     " }\n"
                     " fac(U6$5)/p1\n"
                     "}()";

const char *TPGM(fibrec) = "{\n"
                            "   fib::FN(I6`n;I6)${\n"
                            "       ?{\n"
                            "           <=(n;0)?0\n"
                            "           <(n;3)?1\n"
                            "           fib(n-1)+fib(n-2)\n"
                            "       }\n"
                            "   }\n"
                            "   fib(8)/p1\n"
                            "}()";

const char *TPGM(ackrec) = "{\n"
                           "    ack::FN(U6`m;U6`n;U6)${\n"
                           "        ?{\n"
                           "            =(m;0)?n+1\n"
                           "            &(m>0;n=0)?ack(m-1;U6$1)\n"
                           "            &(m>0;n>0)?ack(m-1;ack(m;n-1))\n"
                           "            n+1\n"
                           "        }\n"
                           "    }\n"
                           "    ack(U6$2;U6$1)/p1\n"
                           "}()";

const char *TPGM(scope) = "{\n"
                          "     s/e::0\n"
                          "     x::NF(I6`x;VD)${\n"
                          "         s+:x\n"
                          "     }\n"
                          "     x(1)\n"
                          "     x(2)\n"
                          "     x(3)\n"
                          "     s/p1\n"
                          "}()";

const char *TPGM(efn) = "{addmul/e::FN${F6(z)*F6(y)+F6(x)}}()";

const char *TPGM(teadd) = "{\n"
                       "    a::[1;0;3]\n"
                       "    a(1): a(0)+a(2)\n"
                       "    a/p1\n"
                       "}()\n";

const char *TPGM(vrmul) = "{\n"
                          "     v::VR$[i::1.1]\n"
                          "     <(i;100)@v,:i*:2\n"
                          "     v/p1\n"
                          "}()\n";

const char *TPGM(st) = "{\n"
                        "   a::1\n"
                        "   b::2.2\n"
                        "   s/e::#{a`a;b`b;$(U6;a+2)`c;\"Hi\"`d}\n"
                        "   s`a: 2 * I6$s`c\n"
                        "   s/p1\n"
                        "}()\n";
