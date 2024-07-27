#!/bin/sh

mkdir ./tests 2> /dev/null

case_type=("varname" "number" "letter" "parentheses-varname" "parentheses-number" "parentheses-letter")
case_operation=("addition" "substraction" "multiplication" "multiplication-no-sign" "power")
for operation in ${case_operation[@]}
do
  for result in ${case_type[@]}
  do
    for lhs in ${case_type[@]}
    do
      for rhs in ${case_type[@]}
      do
        case "$result" in
          "${case_type[0]}")
            TEST_CASE="\$a"
            ;;
          "${case_type[1]}")
            TEST_CASE="5"
            ;;
          "${case_type[2]}")
            TEST_CASE="x"
            ;;
          "${case_type[3]}")
            TEST_CASE="(\$a)"
            ;;
          "${case_type[4]}")
            TEST_CASE="(5)"
            ;;
          "${case_type[5]}")
            TEST_CASE="(x)"
            ;;
        esac

        case "$lhs" in
          "${case_type[0]}")
            TEST_CASE="$TEST_CASE = \$b"
            ;;
          "${case_type[1]}")
            TEST_CASE="$TEST_CASE = 5"
            ;;
          "${case_type[2]}")
            TEST_CASE="$TEST_CASE = y"
            ;;
          "${case_type[3]}")
            TEST_CASE="$TEST_CASE = (\$b)"
            ;;
          "${case_type[4]}")
            TEST_CASE="$TEST_CASE = (5)"
            ;;
          "${case_type[5]}")
            TEST_CASE="$TEST_CASE = (y)"
            ;;
        esac

        case ${operation} in
          "${case_operation[0]}")
            TEST_CASE="$TEST_CASE + "
            ;;
          "${case_operation[1]}")
            TEST_CASE="$TEST_CASE - "
            ;;
          "${case_operation[2]}")
            TEST_CASE="$TEST_CASE * "
            ;;
          "${case_operation[4]}")
            TEST_CASE="$TEST_CASE ^ "
            ;;
        esac

        case "$rhs" in
          "${case_type[0]}")
            TEST_CASE="$TEST_CASE\$c"
            ;;
          "${case_type[1]}")
            TEST_CASE="${TEST_CASE}5"
            ;;
          "${case_type[2]}")
            TEST_CASE="${TEST_CASE}z"
            ;;
          "${case_type[3]}")
            TEST_CASE="$TEST_CASE(\$c)"
            ;;
          "${case_type[4]}")
            TEST_CASE="$TEST_CASE(5)"
            ;;
          "${case_type[5]}")
            TEST_CASE="$TEST_CASE(z)"
            ;;
        esac

        TEST_NAME="$operation-$result-$lhs-$rhs.txt"
        echo "$TEST_CASE" >> "./tests/$TEST_NAME"

        case "$result" in
          "${case_type[0]}"|"${case_type[3]}")
            echo "echo \$a" >> "./tests/$TEST_NAME"
            ;;
        esac
      done
    done
  done
done
