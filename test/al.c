
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
    alf(x);
    alf(y);
    al_f(a);
    return 0;
}
