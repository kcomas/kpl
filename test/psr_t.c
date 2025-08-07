
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
    tkn *t = tkn_b(tkn_i(&pm, &al_te, &al_err, tkn_mktbl, tkn_df, mc_i(0, &al_mc)));
    vr *v = vr_i(10, &al_vr, (void*) te_f);
    bp = psr_b(psr_i(&pm, &al_te, &al_lst, &al_err, psr_entry_f, mktbl, t, v));
    psr_a(bp, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(bp->tt, TOKEN(UN), "Σ", tkn_ft));
}

static __attribute__((destructor)) void psr_des(void) {
    psr_f(bp);
}

psr *bpsr(const char *pgm) {
    return psr_i_psr(bp, mc_i_cstr(pgm, &al_mc));
}

te *psr_r(psr *p) {
    psr_stat pstat;
    te *nh = te_i(3, p->ta, psr_n_err_f);
    err *e = NULL;
    if ((pstat = psr_n(p, nh, &e)) != PSR_STAT(END)) {
        err_p(e, true);
        err_f(e);
        te_f(nh);
        psr_f(p);
        return NULL;
    }
    return psr_g_rn(p, nh);
}

const char *TPGM(aplyopadd) = "+(1;2)";

const char *TPGM(typetype) = "FN(I6`x;F6`y;FN(U6`z;I6))";

const char *TPGM(fnadd3) = "{f::FN(I6`a;I6`b;I6`c;I6)${-a+b+c};f(1;2;3)\\p1}()";

const char *TPGM(fnf6muli6cstadd) = "{f::FN(F6`x;F6`y;U6`z;F6)${+(x*y;F6$z)};f(4.4;6.6;U6$2)\\p1}()";

const char *TPGM(apltypefn) = "{f::FN${I6(a)+I6(b)*a};f(3;4)\\p1}()";

const char *TPGM(facloop) = "{\n\tv::5\n\tf\\e::1\n\t>(v;1)@{\n\t\tf*:v\n\t\tv-:1\n\t}\n\tf\\p1\n}()";

const char *TPGM(facrec) = "{\n"
                     " fac::FN(U6`n;U6)${\n"
                     "  {n>1}?{\n"
                     "   n*fac(n-1)\n"
                     "   U6$1\n"
                     "  }\n"
                     " }\n"
                     " fac(U6$5)\\p1\n"
                     "}()";

const char *TPGM(fibrec) = "{\n"
                            "   fib::FN(I6`n;I6)${\n"
                            "       ?{\n"
                            "           <=(n;0)?0\n"
                            "           <(n;3)?1\n"
                            "           fib(n-1)+fib(n-2)\n"
                            "       }\n"
                            "   }\n"
                            "   fib(8)\\p1\n"
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
                           "    ack(U6$2;U6$1)\\p1\n"
                           "}()";

const char *TPGM(scope) = "{\n"
                          "     s\\e::0\n"
                          "     x::NF(I6`x;VD)${\n"
                          "         s+:x\n"
                          "     }\n"
                          "     x(1)\n"
                          "     x(2)\n"
                          "     x(3)\n"
                          "     s\\p1\n"
                          "}()";

const char *TPGM(efn) = "{addmul\\e::FN${F6(z)*F6(y)+F6(x)}}()";

const char *TPGM(teadd) = "{\n"
                       "    a::[1;0;3]\n"
                       "    a(1): a(0)+a(2)\n"
                       "    a\\p1\n"
                       "}()\n";

const char *TPGM(vrmul) = "{\n"
                          "   v::VR$[i::1.1]\n"
                          "   <(i;100)@v,:i*:2\n"
                          "   v\\p1\n"
                          "}()\n";

const char *TPGM(st) = "{\n"
                        "   a::1\n"
                        "   b::2.2\n"
                        "   s\\e::#{a`a;b`b;$(U6;a+2)`c;\"Hi\"`d}\n"
                        "   s`a: 2 * I6$s`c\n"
                        "   s\\p1\n"
                        "}()\n";

const char *TPGM(import) = "{\n"
                           " i::\"./ex/sum.kpl\"\\l\n"
                           " i\\p1\n"
                           " v\\e::1+i`b*2\n"
                           " v\\p1\n"
                           "}()\n";

const char *TPGM(lfac) = "{\n"
                         " l::\"./ex/fac.kpl\"\\l\n"
                         " e\\e::l`fac(6)\n"
                         " e\\p1\n"
                         "}()\n";

const char *TPGM(istr) = "{\n"
                        "   a::987\n"
                        "   b::3.14\n"
                        "   c::\"a: {}, b: {}\"(a;b)\n"
                        "   c\\p1\n"
                        "}()\n";

const char *TPGM(fnscope) = "{\n"
                            "   b::FN(F6`n;F6)\n"
                            "   a::FN${b(F6(n))}\n"
                            "   b::FN${v::4.3\n"
                            "       f::NF${F6(n)-v}\n"
                            "       f(F6(n))}\n"
                            "   c::FN${a(F6(n))}\n"
                            "   c(9.6)\\p1\n"
                            "}()\n";

const char *TPGM(or) = "{\n"
                       " a::0\n"
                       " b::1\n"
                       " |(a;b)?+(a;b)\\p1\n"
                       "}()\n";

const char *TPGM(un) = "{\n"
                       "    u::1`a\n"
                       "    u:2.2`b\n"
                       "    u\\p1\n"
                       "    u`b\\p1\n"
                       "    u`a\\p1\n"
                       "}()\n";

const char *TPGM(unmatch) = "{\n"
                          " u::1`a\n"
                          " u:2`b\n"
                          " c\\e::u#{\n"
                          "  \"a {}\"(a)`a\n"
                          "  \"b {}\"(b)`b\n"
                          "  \"inv\"\n"
                          "  }\n"
                          " c\\p1\n"
                          "}()\n";

const char *TPGM(ns) = "{\n"
                       " s::\"./ex/sum.kpl\"\n"
                       " f::.f.rs(s)\n"
                       " f`v\\p1\n"
                       "}()\n";

const char *TPGM(iline) = "{.(f;.rs(\"./ex/sum.kpl\"))`v\\p1}()";

const char *TPGM(unsg) = "{a::1`v;a:\"a\"`s;a\\p1}()";

const char *TPGM(vridx) = "{\n"
                          " v\\e::VR$[[\"a\";\"b\"];[\"c\";\"d\"]]\n"
                          " v\\p1\n"
                          " v(-1)\\p1\n"
                          " v(-1)`v(0)\\p1 // c\n"
                          " v(3)#{e\\p1`e}\n"
                          "}()\n";

const char *TPGM(nscope) = "{\n"
                           " x::6.6\n"
                           " fa::NF${\n"
                           "    fb::NF${x*:2.0}\n"
                           "    fb()\n"
                           " }\n"
                           " fa()\n"
                           " x\\p1\n"
                           "}()\n";

const char *TPGM(cj) = "{\n"
                       "    mc::FN${\n"
                       "        c::0\n"
                       "        CJ${c+:1}\n"
                       "    }\n"
                       "    cc::mc()\n"
                       "    cc()\\p1\n"
                       "    cc()\\p1\n"
                       "    e\\e::cc()\n"
                       "}()\n";

const char *TPGM(leap) = "{\n"
                        " y::2000\n"
                        " v\\e::VR(y\\t)$[]\n"
                        " <=(y;2030)@{\n"
                        "    |(&(0=y%4;0!=y%100);0=y%400)?v,:y\n"
                        "    y+:1\n"
                        " }\n"
                        " v\\p1\n"
                        "}()\n";

const char *TPGM(vrup) = "{\n"
                         " v::VR$[1;2;3]\n"
                         " f::FN${\n"
                         "  VR(I6)(a)(1):5\n"
                         "  a\\p1\n"
                         "  a(5):1\n"
                         "  1`v\n"
                         " }\n"
                         " 'f(v)\n"
                         "}()\n";

const char *TPGM(invstll) = "{\n"
                         " l\\e::ST(I6`i;UN(VD`y;l\\t`z)`n)$#{0`i;`y`n}\n"
                         " l\\p1\n"
                         " a::NF${\n"
                         "  \"a: {}\"(I6(i))\\p1\n"
                         "  b::FN(l\\t`x;l\\t)${\"b\"\\p1;x`n#{b(z)`z;x`y}}\n"
                         "  b(l)`n`z:l\\t$#{i`i;`y`n}\n"
                         " }\n"
                         " i::0\n"
                         " <=(i;5)@a(i+:1)\n"
                         " l\\p1\n"
                         "}()\n";

const char *TPGM(stll) = "{\n"
                         " l\\e::ST(I6`i;UN(VD`y;l\\t`z)`n)$#{0`i;`y`n}\n"
                         " a::FN(l\\t`x;I6`i;VD)${\n"
                         "  \"a: {}\"(i)\\p1\n"
                         "  x`n#{\n"
                         "   :(x`n`z;x$#{i`i;`y`n})`y\n"
                         "   a(z;i)`z\n"
                         "  }\n"
                         " }\n"
                         " i::0\n"
                         " <=(i;5)@a(l;i+:1)\n"
                         " l\\p1\n"
                         " b::FN(l\\t`x;VD)${\n"
                         "  \"b: {}\"(x`i)\\p1\n"
                         "  x`n#{\n"
                         "    b(z)`z\n"
                         "   }\n"
                         " }\n"
                         " b(l)\n"
                         "}()\n";

const char *TPGM(validm) = "{\n"
                       " a::VR$[VR$[1;2;3];VR$[4;5;6]]\n"
                       " b::VR$[VR$[1;2];VR$[4]]\n"
                       " v::FN(a\\t`x;A)${\n"
                       "  =(l::/x;0)?'\"Empty\"\n"
                       "  t::/x(0)`v\n"
                       "  (i::U6$1;i<l;i+:1)@?(!=(t;/x(i)`v);'\"Inv\")\n"
                       "  `v\n"
                       " }\n"
                       " 'v(a)\n"
                       " a\\p1\n"
                       " 'v(b)\n"
                       "}()\n";

const char *TPGM(fndiv) = "{\n"
                        " f::FN${/(I6(x);I6(y))`v}\n"
                        " '(f(4;2))\\p1\n"
                        " '(f(1;0))\\p1\n"
                        "}()\n";
