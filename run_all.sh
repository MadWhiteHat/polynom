#!/bin/sh

rm -rf ./output
mkdir output
for testcase in tests/*.txt
do
  valgrind --leak-check=full --show-leak-kinds=all ./polynom $testcase\
    &> output/output-`basename $testcase .txt`.txt
done
