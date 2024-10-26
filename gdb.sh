#!/bin/bash

# access stack in gdb with
# p *((var*)($rbp - (8 * 2))).sg

gdb -ex "b src/jit.c:`cat src/jit.c | grep -n "?? DEB" | cut -d":" -f 1`" -ex "r ./ex/t/flt.kpl" ./jit_test
