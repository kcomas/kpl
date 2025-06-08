#!/bin/bash

# access stack in gdb with
# p *((var*)($rbp - (8 * 2))).sg

set -e

if [[ "$#" -ne 1 ]]
then
    echo "Usage: $0 file.kpl"
    exit 1
fi

if [[ ! -f ./jit_test ]]
then
    make clean && make jit_test
fi

gdb -ex "b src/jit.c:`cat src/jit.c | grep -n "?? DEB" | cut -d":" -f 1`" -ex "r $1" ./jit_test
