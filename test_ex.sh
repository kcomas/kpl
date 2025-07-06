#!/bin/bash

# Test all programs in ex folder
# $1 can be any pgm for benching or mem testing

KPL=./kpl

if [ ! -f $KPL ]
then
    make clean
    make -j$(nproc)
fi

PGMS=$(find ./ex -type f -name "*.kpl")

for P in $PGMS
do
    cat $P
    if [ $1 ]
    then
        bash -c "$1 $KPL $P"
    else
        $KPL $P
    fi
    if [ $? -ne 0 ]
    then
        echo -e "\e[1;91mERROR\e[0m"
        exit 1
    fi
done

echo -e "\e[1;92mSUCCESS\e[0m"
