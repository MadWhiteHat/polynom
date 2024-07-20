#!/bin/sh

list=('./failed/segfault' './failed/lexical' './failed/syntax' './failed/semantics/different' './failed/semantics/other' './failed/semantics/rvalue' './failed/semantics/uninitialized')

for dir in ${list[@]}
do
  echo $dir: $(ls -1 $dir | wc -l)
done
