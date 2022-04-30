#!/bin/bash

if [ "$#" -ne 1 ]; then

    exit 1
fi

# read input
input=$1;
input_size=${#input}
processors=$((2*$input_size - 2));
processors=$(($processors > 0 ? $processors : 0)) # non zero

# compile
mpic++ proj.cpp

# run
mpirun -oversubscribe -np $processors a.out $input

# clean
rm a.out