#!/bin/sh

mem_test() {
  valgrind --leak-check=full --show-leak-kinds=all ./polynom $1\
    &> output/output-$(basename $1)
}

if [ $# -eq 1 ]
then
  mem_test $1
else
  rm -rf ./output
  mkdir output
  for testcase in tests/*.txt
  do
    mem_test $testcase
  done
fi

rm -f ./vgcore.*
