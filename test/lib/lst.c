
#include "../../src/lib/lst.h"

void basic(void) {
    lst *l = lst_i(&malloc, &malloc, &free, NULL, &free);
    lst_ab(l, I6(1));
    lst_ab(l, I6(2));
    lst_ab(l, I6(3));
    lst_f(l);
}

int main(void) {
    basic();
    return 0;
}
