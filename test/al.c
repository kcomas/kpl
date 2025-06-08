
#include "../src/al.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    al *a = al_i();
    char *x = ala(a, 1000);
    strcpy(x, "Hello Word");
    printf("%s\n", x);
    alf(x);
    al_f(a);
    return 0;
}
