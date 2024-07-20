#/bin/sh

rm -rf  sorted

if [ $1 == "output" ]
then 
  mkdir -p sorted/lexical
  mkdir -p sorted/segfault
  mkdir -p sorted/semantics
  mkdir -p sorted/syntax
  mkdir -p sorted/semantics/rvalue
  mkdir -p sorted/semantics/different
  mkdir -p sorted/semantics/uninitialized
  mkdir -p sorted/semantics/polynomial_power
  mkdir -p sorted/semantics/other
  mkdir -p sorted/other

  for filename in ./output/*
  do
    if grep -q "rvalue" $filename
    then
      ln -s $(realpath $filename) ./sorted/semantics/rvalue/$(basename filename)
    elif grep -q "different" $filename
    then
      ln -s $(realpath $filename) ./sorted/semantics/different/$(basename filename)
    elif grep -q "uninitialized" $filename
    then
      ln -s $(realpath $filename) ./sorted/semantics/uninitialized/$(basename filename)
    elif grep -q "power polynomial to polynomial" $filename
    then
      ln -s $(realpath $filename) ./sorted/semantics/polynomial_power/$(basename filename)
    elif grep -q "Lexical" $filename
    then
      ln -s $(realpath $filename) ./sorted/lexical/$(basename filename)
    elif grep -q "Syntax" $filename
    then
      ln -s $(realpath $filename) ./sorted/syntax/$(basename filename)
    elif grep -q "Semantics" $filename
    then
      ln -s $(realpath $filename) ./sorted/semantics/other/$(basename filename)
    elif grep -q "fault" $filename
    then
      ln -s $(realpath $filename) ./sorted/semantics/segfault/$(basename filename)
    else
      ln -s $(realpath $filename) ./sorted/other/$(basename filename)
    fi
  done
elif [ $1 == "memory" ]
then
  mkdir -p sorted/leak
  mkdir -p sorted/no_leak
  for filename in ./output/*
  do
    if grep -q "no leaks are possible" $filename
    then
      ln -s $(realpath $filename) ./sorted/no_leak/$(basename $filename)
    else
      ln -s $(realpath $filename) ./sorted/leak/$(basename $filename)
    fi
  done
fi

