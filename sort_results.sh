#/bin/sh

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
    if [ $(grep -c "rvalue" $filename) -gt 0 ]
    then
      ln -sf $(realpath $filename) ./sorted/semantics/rvalue/$(basename $filename)
    elif [ $(grep -c "different" $filename) -gt 0 ]
    then
      ln -sf $(realpath $filename) ./sorted/semantics/different/$(basename $filename)
    elif [ $(grep -c "uninitialized" $filename) -gt 0 ]
    then
      ln -sf $(realpath $filename) ./sorted/semantics/uninitialized/$(basename $filename)
    elif [ $(grep -c "power polynomial to polynomial" $filename) -gt 0 ]
    then
      ln -sf $(realpath $filename) ./sorted/semantics/polynomial_power/$(basename $filename)
    elif [ $(grep -c "Lexical" $filename) -gt 0 ]
    then
      ln -sf $(realpath $filename) ./sorted/lexical/$(basename $filename)
    elif [ $(grep -c "Syntax" $filename) -gt 0 ]
    then
      ln -sf $(realpath $filename) ./sorted/syntax/$(basename $filename)
    elif [ $(grep -c "Semantics" $filename) -gt 0 ]
    then
      ln -sf $(realpath $filename) ./sorted/semantics/other/$(basename $filename)
    elif [ $(grep -c "fault" $filename) -gt 0 ]
    then
      ln -sf $(realpath $filename) ./sorted/segfault/$(basename $filename)
    else
      ln -sf $(realpath $filename) ./sorted/other/$(basename $filename)
    fi
  done
elif [ $1 == "memory" ]
then
  mkdir -p sorted/leak
  mkdir -p sorted/no_leak
  for filename in ./output/*
  do
    if [ $(grep -c "no leaks are possible" $filename) -gt 0 ]
    then
      ln -sf $(realpath $filename) ./sorted/no_leak/$(basename $filename)
    else
      ln -sf $(realpath $filename) ./sorted/leak/$(basename $filename)
    fi
  done
else
  rm -rf  sorted
fi
