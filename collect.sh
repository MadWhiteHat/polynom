#!/bin/sh

list=('./sorted/other' './sorted/segfault' './sorted/lexical' './sorted/syntax' './sorted/semantics/different' './sorted/semantics/other' './sorted/semantics/rvalue' './sorted/semantics/uninitialized' './sorted/semantics/polynomial_power')
total=0

for dir in ${list[@]}
do
  curr=$(ls -1 $dir | wc -l)
  echo $dir: $curr
  total=$(expr $total + $curr)
done
echo Total: $total

list=('./sorted/no_leak' './sorted/leak')
total=0

for dir in ${list[@]}
do
  curr=$(ls -1 $dir | wc -l)
  echo $dir: $curr
  total=$(expr $total + $curr)
done
echo Total: $total
