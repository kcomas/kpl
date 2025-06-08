
#include "../src/al.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    al *a = al_i();
    char *x = ala(a, 100);
    strcpy(x, "Hello Word");
    printf("%s\n", x);
    char *y = ala(a, 100000);
    strcpy(y, "This is a test");
    printf("%s\n", y);
    char *z = ala(a, 50);
    strcpy(z, "asdf");
    printf("%s\n", z);
    alci *xi = (void*) x - sizeof(alci);
    alci *zi = (void*) z - sizeof(alci);
    alci *yi = (void*) y - sizeof(alci);
    printf("a->len: %lu\n", a->len);
    printf("xzac: %d, xyac: %d, zyac: %d\n", xi->ac == zi->ac, xi->ac == yi->ac, zi->ac == yi->ac);
    alf(x);
    alf(y);
    alf(z);
    al_f(a);
    return 0;
}
