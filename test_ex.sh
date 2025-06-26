#!/bin/bash

# Test all programs in ex folder

KPL=./kpl

if [ ! -f $KPL ]
then
    make clean
    make -j $(nproc)
fi

PGMS=$(find ./ex -type f -name "*.kpl")

for P in $PGMS
do
    cat $P
    $KPL $P
    if [ $? -ne 0 ]
    then
        echo -e "\e[1;91mERROR\e[0m"
        exit 1
    fi
done

echo -e "\e[1;92mSUCCESS\e[0m"
