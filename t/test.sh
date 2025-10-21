#!/usr/bin/env bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")
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
  EXPECTED="$(cat t/example.1.json.expected)"
  ACTUAL="$(cat t/example.1.ldif | bin/jdif)"
  if [ "$EXPECTED" = "$ACTUAL" ]
  then
    printf "\e[32m#$FUNCNAME - PASSED!\e[0m\n"
  else
    printf "\e[31m#$FUNCNAME - FAILED!\e[0m\n"
    echo "$EXPECTED" > $SCRIPT_DIR/../tmp/expected
    echo "$ACTUAL" > $SCRIPT_DIR/../tmp/actual
    diff --color=always $SCRIPT_DIR/../tmp/expected $SCRIPT_DIR/../tmp/actual
    rm $SCRIPT_DIR/../tmp/expected $SCRIPT_DIR/../tmp/actual
    FAILURES=$((FAILURES + 1))
  fi
}

function test_example_stdin_2 () {
  EXAMPLES=$((EXAMPLES + 1))
  EXPECTED="$(cat t/example.2.json.expected)"
  ACTUAL="$(cat t/example.2.ldif | bin/jdif)"
  if [ "$EXPECTED" = "$ACTUAL" ]
  then
    printf "\e[32m#$FUNCNAME - PASSED!\e[0m\n"
  else
    printf "\e[31m#$FUNCNAME - FAILED!\e[0m\n"
    echo "$EXPECTED" > $SCRIPT_DIR/../tmp/expected
    echo "$ACTUAL" > $SCRIPT_DIR/../tmp/actual
    diff --color=always $SCRIPT_DIR/../tmp/expected $SCRIPT_DIR/../tmp/actual
    rm $SCRIPT_DIR/../tmp/expected $SCRIPT_DIR/../tmp/actual
    FAILURES=$((FAILURES + 1))
  fi
}

function test_example_stdin_3 () {
  EXAMPLES=$((EXAMPLES + 1))
  EXPECTED="$(cat t/example.3.json.expected)"
  ACTUAL="$(cat t/example.3.ldif | bin/jdif)"
  if [ "$EXPECTED" = "$ACTUAL" ]
  then
    printf "\e[32m#$FUNCNAME - PASSED!\e[0m\n"
  else
    printf "\e[31m#$FUNCNAME - FAILED!\e[0m\n"
    echo "$EXPECTED" > $SCRIPT_DIR/../tmp/expected
    echo "$ACTUAL" > $SCRIPT_DIR/../tmp/actual
    diff --color=always $SCRIPT_DIR/../tmp/expected $SCRIPT_DIR/../tmp/actual
    rm $SCRIPT_DIR/../tmp/expected $SCRIPT_DIR/../tmp/actual
    FAILURES=$((FAILURES + 1))
  fi
}

function test_example_f_arg () {
  EXAMPLES=$((EXAMPLES + 1))
  EXPECTED="$(cat t/example.1.json.expected)"
  ACTUAL="$(bin/jdif -f t/example.1.ldif)"
  if [ "$EXPECTED" = "$ACTUAL" ]
  then
    printf "\e[32m#$FUNCNAME - PASSED!\e[0m\n"
  else
    printf "\e[31m#$FUNCNAME - FAILED!\e[0m\n"
    echo "Expected:\n\t$EXPECTED\n"
    echo "Actual:\n\t$ACTUAL\n"
    FAILURES=$((FAILURES + 1))
  fi
}

function test_no_file_f_arg () {
  EXAMPLES=$((EXAMPLES + 1))
  EXPECTED="Please specify a file path with \`jdif -f /path/to/file\`"
  ACTUAL="$(bin/jdif -f)"
  if [ "$EXPECTED" = "$ACTUAL" ]
  then
    printf "\e[32m#$FUNCNAME - PASSED!\e[0m\n"
  else
    printf "\e[31m#$FUNCNAME - FAILED!\e[0m\n"
    FAILURES=$((FAILURES + 1))
  fi
}

function test_example_file_arg () {
  EXAMPLES=$((EXAMPLES + 1))
  EXPECTED="$(cat t/example.1.json.expected)"
  ACTUAL="$(bin/jdif --file t/example.1.ldif)"
  if [ "$EXPECTED" = "$ACTUAL" ]
  then
    printf "\e[32m#$FUNCNAME - PASSED!\e[0m\n"
  else
    printf '\e[31m#test_example_stdin - FAILED!\e[0m\n'
    echo "Expected:\n\t$EXPECTED\n"
    echo "Actual:\n\t$ACTUAL\n"
    FAILURES=$((FAILURES + 1))
  fi
}

function test_no_file_file_arg () {
  EXAMPLES=$((EXAMPLES + 1))
  EXPECTED="Please specify a file path with \`jdif -f /path/to/file\`"
  ACTUAL="$(bin/jdif --file)"
  if [ "$EXPECTED" = "$ACTUAL" ]
  then
    printf "\e[32m#$FUNCNAME - PASSED!\e[0m\n"
  else
    printf "\e[31m#$FUNCNAME - FAILED!\e[0m\n"
    FAILURES=$((FAILURES + 1))
  fi
}

function test_unknown_arg () {
  EXAMPLES=$((EXAMPLES + 1))
  EXPECTED="Unsupported option: --foo."
  ACTUAL="$(bin/jdif --foo)"
  if [ "$EXPECTED" = "$ACTUAL" ]
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
test_example_stdin_2
test_example_stdin_3
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