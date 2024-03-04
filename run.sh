#!/bin/bash

clang++ representer.cpp -lclang
./a.out
clang -cc1 -ast-print test.c > representation.txt
while read i j; do sed -i "s/$j/$i/g" representation.txt; done < replace.txt
