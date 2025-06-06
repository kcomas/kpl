
#include "../src/lib/te.h"
#include "t.h"

void node_free(te *t) {
    if (t->d[1].p) te_f(t->d[1].p);
    if (t->d[2].p) te_f(t->d[2].p);
    t->af->f(t);
}

static te *node(const char *v) {
    te *t = te_i(3, &al_te, (void*) node_free);
    t->d[0] = P(v);
    return t;
}

static void node_p(const te *t, size_t idnt) {
    if (!t || !t->d[0].p) return;
    for (size_t i = 0; i < idnt; i++) putchar('-');
    printf(" %s |\n", (char*) t->d[0].p);
    idnt += 2;
    node_p(t->d[1].p, idnt);
    node_p(t->d[2].p, idnt);
}

T(te_tree) {
    te *n = node("5");
    n->d[1].p = node("3");
    ((te*)n->d[1].p)->d[1].p = node("2");
    ((te*)n->d[1].p)->d[2].p = node("4");
    n->d[2].p = node("7");
    ((te*)n->d[2].p)->d[1].p = node("6");
    ((te*)n->d[2].p)->d[2].p = node("8");
    node_p(n, 0);
    A(strcmp(((te*)((te*)n->d[2].p)->d[2].p)->d[0].p, "8") == 0, "tree insert fail");
    te_f(n);
}
