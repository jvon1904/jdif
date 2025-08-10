#!/usr/bin/env bash

EXPECTED='{"name": "Test","age": 99,"uid": "1329692a-9108-47ec-aab1-1a2a4c2663c4","time": "10:55pm","pi": 3.14,"sentence": "The quick brown fox jumps over the lazy dog!","ranking": "#1!"}'
EXAMPLES=0
FAILURES=0

function test_no_input () {
  EXAMPLES=$((EXAMPLES + 1))
  bin/jdif > /dev/null
  if [ "$?" = "0" ]
  then
    printf "\e[32m#$FUNCNAME - PASSED!\e[0m\n"
  else
    printf "\e[31m#$FUNCNAME - FAILED!\e[0m\n"
    FAILURES=$((FAILURES + 1))
  fi
}

function test_help () {
  EXAMPLES=$((EXAMPLES + 1))
  bin/jdif --help > /dev/null
  if [ "$?" = "0" ]
  then
    printf "\e[32m#$FUNCNAME - PASSED!\e[0m\n"
  else
    printf "\e[31m#$FUNCNAME - FAILED!\e[0m\n"
    FAILURES=$((FAILURES + 1))
  fi
}

function test_example_stdin () {
  EXAMPLES=$((EXAMPLES + 1))
  ACTUAL="$(cat example.ldif | bin/jdif)"
  if [ "$EXPECTED" = "$ACTUAL" ]
  then
    printf "\e[32m#$FUNCNAME - PASSED!\e[0m\n"
  else
    echo "Expected:\n\t$EXPECTED\n"
    echo "Actual:\n\t$ACTUAL\n"
    printf "\e[31m#$FUNCNAME - FAILED!\e[0m\n"
    FAILURES=$((FAILURES + 1))
  fi
}

function test_example_f_arg () {
  EXAMPLES=$((EXAMPLES + 1))
  ACTUAL="$(bin/jdif -f example.ldif)"
  if [ "$EXPECTED" = "$ACTUAL" ]
  then
    printf "\e[32m#$FUNCNAME - PASSED!\e[0m\n"
  else
    echo "Expected:\n\t$EXPECTED\n"
    echo "Actual:\n\t$ACTUAL\n"
    printf "\e[31m#$FUNCNAME - FAILED!\e[0m\n"
    FAILURES=$((FAILURES + 1))
  fi
}

function test_no_file_f_arg () {
  EXAMPLES=$((EXAMPLES + 1))
  bin/jdif -f > /dev/null
  if [ "$?" = "1" ]
  then
    printf "\e[32m#$FUNCNAME - PASSED!\e[0m\n"
  else
    printf "\e[31m#$FUNCNAME - FAILED!\e[0m\n"
    FAILURES=$((FAILURES + 1))
  fi
}

function test_example_file_arg () {
  EXAMPLES=$((EXAMPLES + 1))
  ACTUAL="$(bin/jdif --file example.ldif)"
  if [ "$EXPECTED" = "$ACTUAL" ]
  then
    printf "\e[32m#$FUNCNAME - PASSED!\e[0m\n"
  else
    echo "Expected:\n\t$EXPECTED\n"
    echo "Actual:\n\t$ACTUAL\n"
    printf '\e[31m#test_example_stdin - FAILED!\e[0m\n'
  fi
}

function test_no_file_file_arg () {
  EXAMPLES=$((EXAMPLES + 1))
  bin/jdif --file > /dev/null
  if [ "$?" = "1" ]
  then
    printf "\e[32m#$FUNCNAME - PASSED!\e[0m\n"
  else
    printf "\e[31m#$FUNCNAME - FAILED!\e[0m\n"
    FAILURES=$((FAILURES + 1))
  fi
}

function test_unknown_arg () {
  EXAMPLES=$((EXAMPLES + 1))
  bin/jdif --foo > /dev/null
  if [ "$?" = "1" ]
  then
    printf "\e[32m#$FUNCNAME - PASSED!\e[0m\n"
  else
    printf "\e[31m#$FUNCNAME - FAILED!\e[0m\n"
    FAILURES=$((FAILURES + 1))
  fi
}

test_no_input
test_help
test_example_stdin
test_example_f_arg
test_no_file_f_arg
test_example_file_arg
test_no_file_file_arg
test_unknown_arg

printf "\n$EXAMPLES examples, $FAILURES failures\n"

if [ "$FAILURES" != 0 ]
then 
  exit 1
else
  exit 0
fi